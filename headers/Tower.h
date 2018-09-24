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
	virtual bool update_shot(Shot* shot) = 0;
	ENTITYTYPE get_type() override;

	//calls Building::render and renders all shots from the tower
	void render() override;
	
	//all projectiles, that are fired from this tower are updated, shots are fired
	void on_tick() override;
	
	//checks if an enemy is in range of the tower
	bool enemy_in_range(Enemy* enemy, double radius, SDL_Point center) const;

	std::string get_projectile_name() const;
	double get_projectile_speed() const;

protected:
	int mAttack_cooldown; //60 / mAttackspeed
	double mRange;
	double mAttack_speed; //Attacks per second
	double mProjectile_speed;
	Damage mDamage;
	std::string mProjectile_name;
	std::string mTower_name;
	std::vector<Shot*> mShots;//all shots that are fired from the tower
};

