#pragma once
#include "Unit.h"

class Level;


class Enemy :
	public Unit
{
public:
	Enemy(std::string monster_name, int way, Level* Level);
	~Enemy();

	void move();
	void got_through();
	Defense get_defense();
	SDL_Point get_position();
	bool take_damage(Damage *dmg);
	void render();

	bool isDead() const;

private:
	std::vector<SDL_Point> mCheckpoints;
	bool mDead;
	Level* mLevel;
};

