#pragma once
#include "Unit.h"
#include <string>

class Level;


class Enemy :
	public Unit
{
public:
	Enemy(std::string monster_name, int way, Level* level);
	~Enemy();

	//moves the enemy along it's assigned path
	//if it hits the last checkpoint it calls got_through()
	//and sets itself to dead
	void move();
	void got_through();
	Defense get_defense() const;
	SDL_Point get_position() const;
	bool take_damage(Damage *dmg);
	void render(); //calls Unit::render() and shows the health bar with two rectangles

	bool is_dead() const;

private:
	std::vector<SDL_Point> mCheckpoints;
	bool mDead;
	Level* mLevel;
	
	//TODO: make healthbar it's own class? not sure
	LTexture* mFull_health_bar;
	LTexture* mEmpty_health_bar;
	SDL_Rect mHealth_bar_dimensions;
};

