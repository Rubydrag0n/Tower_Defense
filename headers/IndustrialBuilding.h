#pragma once
#include "Building.h"
#include "Level.h"

class IndustrialBuilding :
	public Building
{
public:
	IndustrialBuilding(std::string industrialBuilding_name, SDL_Point coords, Level* level);
	~IndustrialBuilding();
	void update();


private:
	Ressources mRessources_produced_per_second;
};

