#include "Tower.h"
#include "ConfigFile.h"
#include "Enemy.h"
#include <SDL.h>


Tower::Tower(std::string tower_name, SDL_Point coords, Level *level) : Building(tower_name, coords, level)
{
	ConfigFile cf("config/game.cfg");
	mTower_name = tower_name;

	mDamage.set_damages(cf.Value(mTower_name + "/stats", "phys"),
		cf.Value(mTower_name + "/stats", "magic"),
		cf.Value(mTower_name + "/stats", "fire"),
		cf.Value(mTower_name + "/stats", "water"),
		cf.Value(mTower_name + "/stats", "elec"));

	mRange = cf.Value(mTower_name + "/stats", "range");
	mAttack_speed = cf.Value(mTower_name + "/stats", "attackspeed");
	mProjectile_speed = cf.Value(mTower_name + "/stats", "projectilespeed");
	mProjectile_name.assign(cf.Value(mTower_name + "/stats", "projectile_name"));
	mAttack_cooldown = 60 / mAttack_speed;
	mElapsed_ticks = 0;
}

Tower::~Tower()
{
}

void Tower::render() const
{
	Building::render();
	for (auto i = 0; i < mShots.size(); i++)
	{
		this->render_shot(mShots.at(i));
	}
}

void Tower::update(std::vector<Enemy*> all_enemies)
{
	if (mElapsed_ticks == 0)
	{
		while (!all_enemies.empty() && mElapsed_ticks == 0)
		{
			if (enemy_in_range(all_enemies.at(0), mRange, mCoords))
			{
				mShots.push_back(this->create_shot(all_enemies.at(0)));
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
void Tower::shoot(std::vector<Enemy*> all_enemies)
{
	for (auto i = 0; i<mShots.size(); i++)
	{
		if(this->update_shot(mShots.at(i), all_enemies))
		{
			mShots.erase(mShots.begin() + i);
		}
	}
}



//checks if an enemy is in range of the tower
bool Tower::enemy_in_range(Enemy* enemy, double radius, SDL_Point center) const
{
	auto x_div = center.x - enemy->get_position().x;
	auto y_div = center.y - enemy->get_position().y;
	auto dist_to_enemy = sqrt(x_div * x_div + y_div * y_div);
	if(dist_to_enemy <= radius)
	{
		return true;
	}
	return false;
}



std::string Tower::get_projectile_name() const
{
	return mProjectile_name;
}

double Tower::get_projectile_speed() const
{
	return mProjectile_speed;
}

