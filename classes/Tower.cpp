#include "Tower.h"



Tower::Tower(std::string tower_name, SDL_Point coords) : Building(tower_name, coords)
{
	mTower_name = tower_name;
}

Tower::~Tower()
{


}
