#pragma once
#include "Unit.h"
#include <string>
#include "Resources.h"

class Level;


class Enemy :
	public Unit
{
public:
	Enemy(std::string monster_name, int way, Level* level);
	~Enemy();

	void on_tick() override;
	
	//moves the enemy along it's assigned path
	//if it hits the last checkpoint it calls got_through()
	//and sets itself to dead
	void move();
	void got_through();
	Defense get_defense() const;
	SDL_Point get_position() const;
	bool take_damage(Damage *dmg);
	//is called once on death
	void on_death();
	void render(); //calls Unit::render() and shows the health bar with two rectangles

	bool is_dead() const;
	ENTITYTYPE get_type() override;

private:
	std::vector<SDL_Point> mCheckpoints;
	bool mDead;
	Level* mLevel;
	//how much life will be lost when the enemy gets through
	int mLife_cost;
	Resources mLoot_resources;
	
	//TODO: make healthbar it's own class? not sure
	LTexture* mFull_health_bar;
	LTexture* mEmpty_health_bar;
	SDL_Rect mHealth_bar_dimensions;
};
