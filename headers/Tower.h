#pragma once
#include "Building.h"
#include "Damage.h"
#include "Enemy.h"
#include "Shot.h"
#include <SDL.h>
#include <vector>

class Tower : 
	public Building
{
public:
	Tower(std::string tower_name, SDL_Point coords, Level* level);
	~Tower();

	bool enemy_in_range(Enemy* enemy, double radius, SDL_Point center);
	void render();

	std::string get_projectile_name();
	double get_projectile_speed();

protected:
	int mElapsed_ticks;
	int mAttack_cooldown;
	double mRange;
	double mAS; //Attacks per second
	double mProjectile_speed;
	Damage mDamage;
	std::string mProjectile_name;
	std::string mTower_name;




private:

};

