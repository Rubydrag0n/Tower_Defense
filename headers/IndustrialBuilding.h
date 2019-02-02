#pragma once
#include "Building.h"
#include "Level.h"

class IndustrialBuilding :
	public Building
{
public:
	IndustrialBuilding(std::string industrial_building_name, SDL_Point coords, Level* level);

	//calls Building::update and adds resources
	void on_tick() override;

	//calls Building::render
	void render() override;

	BUILDINGTYPE get_building_type() override;
};

