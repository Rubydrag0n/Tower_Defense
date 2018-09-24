#include "../headers/Enemy.h"
#include "ConfigFile.h"
#include "SDL_setup.h"
#include "Level.h"
#include "LayerHandler.h"

//needs the level name for getting the movement checkpoints from the config file
Enemy::Enemy(std::string monster_name, int way, Level* level) : Unit(monster_name)
{
	mLevel = level;
	//way is the index of the way the unit is to run (there can be multiple ones in one level) (starts with 0)
	auto s_way = std::to_string(way);
	//printf("reading checkpointnumber...\n");
	int checkpoint_number = gConfig_file->Value("level" + mLevel->get_level_number(), "way" + s_way + "number_of_checkpoints");

	for (auto i = 0; i < checkpoint_number; i++)
	{
		//reading the checkpoint data
		auto number = std::to_string(i);
		SDL_Point p;

		//printf("reading checkpoint %i\n", i);
		p.x = gConfig_file->Value("level" + mLevel->get_level_number(), "way" + s_way + "checkpoint" + number + "x");
		p.y = gConfig_file->Value("level" + mLevel->get_level_number(), "way" + s_way + "checkpoint" + number + "y");
		//sorting the checkpoints into the array (they have to be in the right order)
		mCheckpoints.push_back(p);
	}
	//starting position is the first checkpoint
	mPosition = mCheckpoints.at(0);
	//delete first element from vector
	mCheckpoints.erase(mCheckpoints.begin());
	mDead = false;

	//initialize health bar things
	std::string health_bar_name = gConfig_file->Value(monster_name + "/sprite", "health_bar");
	mEmpty_health_bar = gTextures->get_texture(gConfig_file->Value(health_bar_name + "/sprite", "empty_path"));
	mFull_health_bar = gTextures->get_texture(gConfig_file->Value(health_bar_name + "/sprite", "full_path"));
	mHealth_bar_dimensions.x = 0;
	mHealth_bar_dimensions.y = 0;
	mHealth_bar_dimensions.w = gConfig_file->Value(health_bar_name + "/sprite", "image_width");
	mHealth_bar_dimensions.h = gConfig_file->Value(health_bar_name + "/sprite", "image_height");
}

Enemy::~Enemy()
{
}

void Enemy::on_tick()
{
}

void Enemy::move()
{
	if (!mDead)
	{
		auto travel_dist = mMoveSpeed / 60.0;

		auto dist_to_next_checkpoint = (mPosition.x - mCheckpoints.at(0).x) * (mPosition.x - mCheckpoints.at(0).x) +
			(mPosition.y - mCheckpoints.at(0).y) * (mPosition.y - mCheckpoints.at(0).y);
		dist_to_next_checkpoint = sqrt(dist_to_next_checkpoint);

		//if the next checkpoint will be reached in this tick
		if (dist_to_next_checkpoint < travel_dist)
		{
			//just set it there
			mPosition.x = mCheckpoints.at(0).x;
			mPosition.y = mCheckpoints.at(0).y;
			//delete checkpoint from the stack
			mCheckpoints.erase(mCheckpoints.begin());
			if (mCheckpoints.empty())
			{
				got_through();
				return;
			}
			return;
		}
		//normally:
		//TODO: make this with alpha and stuff
		//for now just moving in x XOR y direction
		if (mPosition.x == mCheckpoints.at(0).x)
		{
			//x is right, move y
			if (mCheckpoints.at(0).y > mPosition.y)
			{
				//position needs to get bigger
				mPosition.y += travel_dist;
				mDirection = DIRECTION::DOWN;
			}
			else
			{
				mPosition.y -= travel_dist;
				mDirection = DIRECTION::UP;
			}
			return;
		}
		//other way around
		if (mCheckpoints.at(0).x > mPosition.x)
		{
			//position needs to get bigger
			mPosition.x += travel_dist;
			mDirection = DIRECTION::RIGHT;
		}
		else
		{
			mPosition.x -= travel_dist;
			mDirection = DIRECTION::LEFT;
		}
	}
}

void Enemy::got_through()
{
	//TODO: Remove Life or something
	//kill the unit
	mLevel->set_lives(mLevel->get_lives() - 1);
	mDead = true;
}

bool Enemy::is_dead() const
{
	return mDead;
}

Defense Enemy::get_defense() const
{
	return mDefense;
}

SDL_Point Enemy::get_position() const
{
	return mPosition;
}

bool Enemy::take_damage(Damage *dmg)
{
	if(mDefense.take_damage(dmg))
	{
		mDead = true;
		return true;
	}
	return false;
}


void Enemy::render()
{
	//rendering the unit
	Unit::render();

	//now rendering the health bar
	SDL_Rect full_health;
	full_health.x = mPosition.x - mHealth_bar_dimensions.w / 2;
	full_health.y = mPosition.y - mCurrent_clip.h / 2 - mHealth_bar_dimensions.h / 2;
	full_health.w = mHealth_bar_dimensions.w;
	full_health.h = mHealth_bar_dimensions.h;

	auto current_health = full_health;
	current_health.w = mHealth_bar_dimensions.w * (this->get_defense().get_health()/this->get_defense().get_full_health());
	auto src_current_health = current_health;
	src_current_health.x = 0;
	src_current_health.y = 0;

	gLayer_handler->render_to_layer(mEmpty_health_bar, LAYERS::WINDOWS, &mHealth_bar_dimensions, &full_health);
	gLayer_handler->render_to_layer(mFull_health_bar, LAYERS::WINDOWS, &src_current_health, &current_health);
}

ENTITYTYPE Enemy::get_type()
{
	return ENTITYTYPE::ENEMY;
}
