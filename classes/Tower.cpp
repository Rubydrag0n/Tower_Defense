#include "Tower.h"
#include "ConfigFile.h"



Tower::Tower(std::string tower_name) : Building(tower_name)
{
	mTower_name = tower_name;
}

Tower::~Tower()
{
}

void Tower::placeTower(int x, int y)
{
	mPosition.x = x;
	mPosition.y = y;
}