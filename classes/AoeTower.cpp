#include "AoeTower.h"
#include "AoeShot.h"
#include "ConfigFile.h"


AoeTower::AoeTower(std::string tower_name, SDL_Point coords, Level* level) : Tower(tower_name, coords, level)
{
	ConfigFile cf("config/game.cfg");
	mExplosiveRadius = cf.Value(mTower_name + "/stats", "explosiveradius");
}


AoeTower::~AoeTower()
{
}

void AoeTower::render()
{
	Tower::render();
	for (auto i = 0; i<mShots.size(); i++)
	{
		mShots.at(i)->render(mShots.at(i)->get_location_to_shoot());
	}
}

//checks if the tower can shoot at an enemy
void AoeTower::update(std::vector<Enemy*> all_enemies)
{
	if (mElapsed_ticks == 0)
	{
		while (!all_enemies.empty() && mElapsed_ticks == 0)
		{
			if (enemy_in_range(all_enemies.at(0), mRange))
			{
				auto shot = new AoeShot(this, all_enemies.at(0)->get_position());
				mShots.push_back(shot);
				mElapsed_ticks = mAttack_cooldown;
			}
			all_enemies.erase(all_enemies.begin());
		}
	}
	else
	{
		mElapsed_ticks--;
	}
	this->shoot(all_enemies);
}

//all projectiles, that are fired from this tower are updated
void AoeTower::shoot(std::vector<Enemy*> all_enemies)
{
	std::vector<Enemy*> copy = all_enemies;
	for (auto i = 0; i<mShots.size(); i++)
	{
		if (mShots[i]->follow())
		{
			while (!all_enemies.empty())
			{
				if (enemy_in_range(all_enemies.at(0), mExplosiveRadius))
				{
					all_enemies.at(0)->take_damage(&mDamage);
				}
				all_enemies.erase(all_enemies.begin());
			}
			delete mShots[i];
			mShots.erase(mShots.begin() + i);
		}
		all_enemies = copy;
	}
}

