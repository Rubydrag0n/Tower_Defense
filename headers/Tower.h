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

	virtual void render_shot(Shot* shot) const = 0;
	virtual Shot* create_shot(Enemy* enemy) = 0;
	bool virtual update_shot(Shot* shot, std::vector<Enemy*> all_enemies) = 0;

	//calls Building::render and renders all shots from the tower
	void render() const override;
	//looks if an enemy is in range, if true: creates shots
	void shoot(std::vector<Enemy*> all_enemies);
	//all projectiles, that are fired from this tower are updated
	void update(std::vector<Enemy*> all_enemies);
	//checks if an enemy is in range of the tower
	bool enemy_in_range(Enemy* enemy, double radius, SDL_Point center) const;

	std::string get_projectile_name() const;
	double get_projectile_speed() const;

protected:
	int mElapsed_ticks; //counts the ticks
	int mAttack_cooldown; //60 / mAttackspeed
	double mRange;
	double mAttack_speed; //Attacks per second
	double mProjectile_speed;
	Damage mDamage;
	std::string mProjectile_name;
	std::string mTower_name;
	std::vector<Shot*> mShots;//all shots that are fired from the tower
};

