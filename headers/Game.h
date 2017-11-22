#pragma once
#include <vector>
#include "Enemy.h"
#include "Tower.h"

class Game 
{
public:
	Game();
	~Game();

	void init_game();
	void start_game();
	void render_all();

	void add_enemies(std::vector<Enemy*> enemies);
	void add_towers(std::vector<Tower*> towers);

private:
	std::vector<Tower*> mAll_towers;
	std::vector<Enemy*> mAll_enemies;
};
