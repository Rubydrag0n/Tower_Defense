#pragma once
#include <vector>
#include "Enemy.h"
#include "Tower.h"
#include "IndustrialBuilding.h"
#include "Menu.h"
#include "Map.h"

class Game 
{
public:
	Game();
	~Game();

	void start_game();
	void render_all();
	void update_all();

	void add_enemies(std::vector<Enemy*> enemies);
	void add_tower(Tower* tower);
	void add_industrial_building(IndustrialBuilding* industrial_building);

private:
	std::vector<Tower*> mAll_towers;
	std::vector<Enemy*> mAll_enemies;
	std::vector<IndustrialBuilding*> mAll_industrial_buildings;
	Menu* mMenu;
	Level* mLevel;
	Map* mMap;
};
