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

	void shoot();
	void update(std::vector<Enemy*> all_enemies);
	bool enemy_in_range(Enemy* enemy);
	void render();

	std::string get_projectile_name();
	double get_projectile_speed();



private:
	std::string mTower_name;
	Damage mDamage;
	double mRange;
	double mAS; //Attacks per second
	double mProjectile_speed;
	std::string mProjectile_name;
	int mAttack_cooldown;
	int mElapsed_ticks;
	std::vector<Shot*> mShots;
};

