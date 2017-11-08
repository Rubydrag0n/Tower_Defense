#include "Tower.h"
#include "ConfigFile.h"
#include "Enemy.h"
#include "Shot.h"
#include <iostream>
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
	mAS = cf.Value(mTower_name + "/stats", "attackspeed");
	mProjectile_speed = cf.Value(mTower_name + "/stats", "projectilespeed");
	mProjectile_name.assign(cf.Value(mTower_name + "/stats", "projectile_name"));
	mAttack_cooldown = 60 / mAS;
	mElapsed_ticks = 0;
}

Tower::~Tower()
{
}

void Tower::render()
{
	std::cout << "render Tower" << std::endl;
	Building::render();
	for(auto i = 0; i<mShots.size(); i++)
	{
		mShots.at(i)->render();
	}
}

//checks if the tower can shoot at an enemy
void Tower::update(std::vector<Enemy*> all_enemies)
{
	if (mElapsed_ticks == 0)
	{
		while (!all_enemies.empty() && mElapsed_ticks == 0)
		{
			if (enemy_in_range(all_enemies.at(0)))
			{
				auto shot = new Shot(this, all_enemies.at(0));
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

//checks if an enemy is in range of the tower
bool Tower::enemy_in_range(Enemy* enemy)
{
	auto x_div = mCoords.x - enemy->get_position().x;
	auto y_div = mCoords.y - enemy->get_position().y;
	auto dist_to_enemy = sqrt(x_div * x_div + y_div * y_div);
	if(dist_to_enemy <= mRange)
	{
		return true;
	}
	return false;
}

//all projectiles, that are fired from this tower are updated
void Tower::shoot()
{ 
	for(auto i=0; i<mShots.size(); i++ )
	{
		if(mShots[i]->get_enemy_to_shoot()->isDead())
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

std::string Tower::get_projectile_name()
{
	return mProjectile_name;
}

double Tower::get_projectile_speed()
{
	return mProjectile_speed;
}

