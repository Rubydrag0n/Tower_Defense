#pragma once
#include "Unit.h"
class Enemy :
	public Unit
{
public:
	Enemy(std::string monster_name, std::string level, int way);
	~Enemy();

	void move();
	void got_through();
	Defense get_defense();
	SDL_Point get_position();

	bool isDead() const;

private:
	std::vector<SDL_Point> mCheckpoints;
	bool mDead;
};

