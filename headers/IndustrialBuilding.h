#pragma once
#include "Building.h"
class IndustrialBuilding :
	public Building
{
public:
	IndustrialBuilding(std::string industrialBuilding_name, SDL_Point coords);
	~IndustrialBuilding();
};

