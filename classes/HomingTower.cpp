#include "HomingTower.h"
#include "HomingShot.h"


HomingTower::HomingTower(std::string tower_name, SDL_Point coords, Level* level) : Tower(tower_name, coords, level)
{
}


HomingTower::~HomingTower()
{
}

void HomingTower::render()
{
	Tower::render();
	for (auto i = 0; i<mShots.size(); i++)
	{
		mShots.at(i)->render(mShots.at(i)->get_enemy_to_shoot()->get_position());
	}
}

//checks if the tower can shoot at an enemy
void HomingTower::update(std::vector<Enemy*> all_enemies)
{
	if (mElapsed_ticks == 0)
	{
		while (!all_enemies.empty() && mElapsed_ticks == 0)
		{
			if (enemy_in_range(all_enemies.at(0), mRange, mCoords))
			{
				auto shot = new HomingShot(this, all_enemies.at(0));
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
	this->shoot();
}

//all projectiles, that are fired from this tower are updated
void HomingTower::shoot()
{
	for (auto i = 0; i<mShots.size(); i++)
	{
		if (mShots[i]->get_enemy_to_shoot()->isDead())
		{
			delete mShots[i];
			mShots.erase(mShots.begin() + i);
			continue;
		}
		if (mShots[i]->follow())
		{
			mShots[i]->get_enemy_to_shoot()->take_damage(&mDamage);
			delete mShots[i];
			mShots.erase(mShots.begin() + i);
		}
	}
}
