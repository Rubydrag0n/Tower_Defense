#pragma once
#include "Building.h"
#include "Level.h"

class IndustrialBuilding :
	public Building
{
public:
	IndustrialBuilding(std::string industrial_building_name, SDL_Point coords, Level* level);
	~IndustrialBuilding();

	//calls Building::update and adds ressources
	void on_tick() override;

	//calls Buildung::render
	void render() override;


private:
	Ressources mRessources_produced_per_second;
};

