#pragma once
#include "Building.h"
#include "Level.h"

class IndustrialBuilding :
	public Building
{
public:
	IndustrialBuilding(std::string industrial_building_name, SDL_Point coords, Level* level, LAYERS click_layer, LAYERS render_layer);

	BUILDINGTYPE get_building_type() override;
};
