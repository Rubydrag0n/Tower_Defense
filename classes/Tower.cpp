#include "Tower.h"
#include <SDL.h>
#include "SDL_setup.h"


Tower::Tower(std::string tower_name, SDL_Point coords) : Building(tower_name, coords)
{
	mTower_name = tower_name;
}

Tower::~Tower()
{
}
