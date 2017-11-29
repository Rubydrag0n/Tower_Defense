#pragma once
#include "Unit.h"
#include <string>

class Level;


class Enemy :
	public Unit
{
public:
	Enemy(std::string monster_name, int way, Level* Level);
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

	bool isDead() const;

private:
	std::vector<SDL_Point> mCheckpoints;
	bool mDead;
	Level* mLevel;
};

