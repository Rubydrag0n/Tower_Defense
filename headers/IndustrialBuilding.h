#pragma once
#include "Building.h"
#include "Level.h"
#include "Menu.h"

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

	BUILDINGTYPE get_building_type() override;

private:
	Resources mRessources_produced_per_second;
};

