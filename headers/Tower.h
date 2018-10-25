#pragma once
#include <SDL.h>
#include <vector>

#include "Building.h"
#include "Damage.h"
#include "Enemy.h"
#include "Shot.h"
#include "Menu.h"

class Tower : 
	public Building
{
public:
	Tower(std::string tower_name, SDL_Point coords, Level* level);
	~Tower();

	virtual void create_shot(Enemy* enemy) = 0;

	//calls Building::render and renders all shots from the tower
	void render() override;
	
	//all projectiles, that are fired from this tower are updated, shots are fired
	void on_tick() override;

	void upgrade(std::string tower_upgrade_section) override;
	void upgrade_damage();
	void upgrade_range();
	void upgrade_attackspeed();

	BUILDINGTYPE get_building_type() override;
	
	//checks if an enemy is in range of the tower
	static bool enemy_in_range(Enemy* enemy, double radius, SDL_Point center);

	std::string get_projectile_name() const;
	double get_projectile_speed() const;

	Damage get_damage();

protected:
	int mAttack_cooldown; //60 / mAttackspeed
	double mRange;
	double mAttack_speed; //Attacks per second
	double mProjectile_speed;
	Damage mDamage;
	std::string mProjectile_name;
	std::string mTower_name;
	LTexture* mRange_indicator_sprite;
};

