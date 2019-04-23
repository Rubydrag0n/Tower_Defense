#pragma once
#include <SDL.h>
#include <vector>

#include "Building.h"
#include "Damage.h"
#include "Enemy.h"

class Tower : 
	public Building
{
public:
	Tower(const std::string& tower_name, SDL_Point coords, Level* level, LAYERS click_layer, LAYERS render_layer);

	virtual void create_shot(Enemy* enemy) = 0;

	//calls Building::render and renders all shots from the tower
	void render() override;
	
	//all projectiles, that are fired from this tower are updated, shots are fired
	void on_tick() override;

	void upgrade(const std::string& tower_upgrade_section) override;
	void upgrade_damage();
	void upgrade_range();
	void upgrade_attack_speed();

	double get_attack_speed() const;
	double get_range() const;

	BUILDINGTYPE get_building_type() override;
	
	//checks if an enemy is in range of the tower
	static bool enemy_in_range(Enemy* enemy, double radius, SDL_Point center);

	std::string get_projectile_name() const;
	double get_projectile_speed() const;

	Damage get_damage() const;

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

