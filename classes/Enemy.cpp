#include <cstdlib>
#include "Enemy.h"
#include "ConfigFile.h"
#include "SDL_setup.h"
#include "Level.h"
#include "LayerHandler.h"
#include "CoordinatesInDouble.h"
#include "Particles.h"
#include "HomingShot.h"

//needs the level name for getting the movement checkpoints from the config file
Enemy::Enemy(std::string monster_name, int way, Level* level) : Unit(monster_name), mDead(false)
{
	mLevel = level;
	//way is the index of the way the unit is to run (there can be multiple ones in one level) (starts with 0)
	auto s_way = std::to_string(way);

	auto level_section = "level" + mLevel->get_level_number();
	auto monster_sprite_section = monster_name + "/sprite";
	auto monster_stats_section = monster_name + "/stats";

	this->mLife_cost = gConfig_file->Value(monster_stats_section, "lifecost");
	int checkpoint_number = gConfig_file->Value(level_section, "way" + s_way + "number_of_checkpoints");

	SDL_Point mOffset;
	mOffset.x = (rand() % 31) - 15;
	mOffset.y = (rand() % 31) - 15;

	for (auto i = 0; i < checkpoint_number; i++)
	{
		//reading the checkpoint data
		auto number = std::to_string(i);
		SDL_Point p;

		//printf("reading checkpoint %i\n", i);
		//take offset into account
		p.x = gConfig_file->Value(level_section, "way" + s_way + "checkpoint" + number + "x") + mOffset.x;
		p.y = gConfig_file->Value(level_section, "way" + s_way + "checkpoint" + number + "y") + mOffset.y;
		//sorting the checkpoints into the array (they have to be in the right order)
		mCheckpoints.push_back(p);
	}
	//starting position is the first checkpoint
	mPosition = static_cast<CoordinatesInDouble>(mCheckpoints.at(0));
	//delete first element from vector
	mCheckpoints.erase(mCheckpoints.begin());

	//initialize health bar things
	std::string health_bar_name = gConfig_file->Value(monster_sprite_section, "health_bar");

	auto healthbar_sprite_section = health_bar_name + "/sprite";
	mEmpty_health_bar = gTextures->get_texture(gConfig_file->Value(healthbar_sprite_section, "empty_path"));
	mFull_health_bar = gTextures->get_texture(gConfig_file->Value(healthbar_sprite_section, "full_path"));
	mHealth_bar_dimensions.x = 0;
	mHealth_bar_dimensions.y = 0;
	mHealth_bar_dimensions.w = gConfig_file->Value(healthbar_sprite_section, "image_width");
	mHealth_bar_dimensions.h = gConfig_file->Value(healthbar_sprite_section, "image_height");


	mLoot_resources.set_resources(gConfig_file->value_or_zero(monster_stats_section, "goldloot"),
		gConfig_file->value_or_zero(monster_stats_section, "woodloot"),
		gConfig_file->value_or_zero(monster_stats_section, "stoneloot"),
		gConfig_file->value_or_zero(monster_stats_section, "ironloot"),
		gConfig_file->value_or_zero(monster_stats_section, "energyloot"),
		gConfig_file->value_or_zero(monster_stats_section, "waterloot"),
		gConfig_file->value_or_zero(monster_stats_section, "foodloot"));
}

Enemy::~Enemy()
{
	if (!mFollowed_by.empty()) {
		for (auto i = mFollowed_by.size() - 1; i > 0; --i)
		{
			delete mFollowed_by.at(i);
		}
		mFollowed_by.clear();
	}
}

void Enemy::on_tick()
{
}

void Enemy::move()
{
	if (!mDead)
	{
		auto travel_dist = mMove_speed / 60.0;	//60.0 is framerate

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
				this->got_through();
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
	//kill the unit
	mLevel->set_lives(mLevel->get_lives() - this->mLife_cost);
	mDead = true;
	this->on_death();
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
	return static_cast<SDL_Point>(*const_cast<CoordinatesInDouble*>(&mPosition));
}

bool Enemy::take_damage(Damage *dmg)
{
	if(mDefense.take_damage(dmg) && !this->is_dead())
	{
		mDead = true;
		this->on_death();
		return true;
	}
	return false;
}

void Enemy::on_death()
{
	mLevel->get_resources()->add(&mLoot_resources);
	new Particle("zombie_death", mPosition, CoordinatesInDouble(), this->get_rotation_angle(), 0.0);
	if (!mFollowed_by.empty()) {
		for (int i = mFollowed_by.size() - 1; i >= 0; i--)
		{
			mFollowed_by.at(i)->self_destruct();
		}
		mFollowed_by.clear();
	}
	this->set_rendering_enabled(false);
}

void Enemy::render()
{
	//rendering the unit
	Unit::render();

	//now rendering the health bar
	SDL_Rect full_health;
	full_health.x = mPosition.x - mHealth_bar_dimensions.w / 2;
	full_health.y = mPosition.y - mCurrent_clip.h - mHealth_bar_dimensions.h / 2;
	full_health.w = mHealth_bar_dimensions.w;
	full_health.h = mHealth_bar_dimensions.h;

	auto current_health = full_health;
	current_health.w = mHealth_bar_dimensions.w * (this->get_defense().get_health() / this->get_defense().get_full_health());
	auto src_current_health = current_health;
	src_current_health.x = 0;
	src_current_health.y = 0;

	gLayer_handler->render_to_layer(mEmpty_health_bar, LAYERS::WINDOWS, &mHealth_bar_dimensions, &full_health);
	gLayer_handler->render_to_layer(mFull_health_bar, LAYERS::WINDOWS, &src_current_health, &current_health);
}

void Enemy::add_following_shot(HomingShot * shot)
{
	this->mFollowed_by.emplace_back(shot);
}

void Enemy::delete_following_shot(HomingShot * shot)
{
	for (auto it = mFollowed_by.begin(); it != mFollowed_by.end(); ++it)
	{
		if ((*it) == shot) {
			(*it)->self_destruct();
			this->mFollowed_by.erase(it);
			return;
		}
	}
}

ENTITYTYPE Enemy::get_type()
{
	return ENTITYTYPE::ENEMY;
}
