#include "../headers/Enemy.h"
#include "ConfigFile.h"

//needs the level name for getting the movement checkpoints from the config file
Enemy::Enemy(std::string monster_name, std::string level, int way) : Unit(monster_name)
{
	ConfigFile cf("config/game.cfg");
	
	//way is the index of the way the unit is to run (there can be multiple ones in one level) (starts with 0)
	auto s_way = std::to_string(way);
	printf("reading checkpointnumber...\n");
	int checkpoint_number = cf.Value(level, "way" + s_way + "number_of_checkpoints");

	for (auto i = 0; i < checkpoint_number; i++)
	{
		//reading the checkpoint data
		auto number = std::to_string(i);
		SDL_Point p;

		printf("reading checkpoint %i\n", i);
		p.x = cf.Value(level, "way" + s_way + "checkpoint" + number + "x");
		p.y = cf.Value(level, "way" + s_way + "checkpoint" + number + "y");
		//sorting the checkpoints into the array (they have to be in the right order)
		mCheckpoints.push_back(p);
	}
	//starting position is the first checkpoint
	mPosition = mCheckpoints.at(0);
	//delete first element from vector
	mCheckpoints.erase(mCheckpoints.begin());
	mDead = false;
}




Enemy::~Enemy()
{
}

//moves the enemy along it's assigned path
//if it hits the last checkpoint it calls got_through()
//and sets itself to dead
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
	mDead = true;
}

bool Enemy::isDead() const
{
	return mDead;
}

Defense Enemy::get_defense()
{
	return mDefense;
}

SDL_Point Enemy::get_position()
{
	return mPosition;
}
