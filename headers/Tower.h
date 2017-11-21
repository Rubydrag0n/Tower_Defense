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

	bool enemy_in_range(Enemy* enemy, double radius, SDL_Point center) const;
	virtual void render_shot(Shot* shot) const = 0;
	virtual Shot* create_shot(Enemy* enemy) = 0;
	void shoot(std::vector<Enemy*> all_enemies);
	bool virtual update_shot(Shot* shot, std::vector<Enemy*> all_enemies) = 0;
	void render() const override;
	void update(std::vector<Enemy*> all_enemies);

	std::string get_projectile_name() const;
	double get_projectile_speed() const;

protected:
	int mElapsed_ticks;
	int mAttack_cooldown;
	double mRange;
	double mAttack_speed; //Attacks per second
	double mProjectile_speed;
	Damage mDamage;
	std::string mProjectile_name;
	std::string mTower_name;
	std::vector<Shot*> mShots;
};

