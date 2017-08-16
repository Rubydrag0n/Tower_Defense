#pragma once
#include "Building.h"
#include <SDL.h>

class Tower : 
	public Building
{
public:
	Tower(std::string tower_name, SDL_Point coords);
	~Tower();


private:
	std::string mTower_name;
};

