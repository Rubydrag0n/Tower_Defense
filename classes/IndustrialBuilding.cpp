#include "IndustrialBuilding.h"
#include "ConfigFile.h"
#include "SDL_setup.h"

IndustrialBuilding::IndustrialBuilding(std::string industrial_building_name, SDL_Point coords, Level* level) : Building(
	std::move(industrial_building_name), coords, level)
{
	SDL_Rect rect;
	rect.x = mCoords.x;
	rect.y = mCoords.y - 200;
	rect.w = 200;
	rect.h = 200;
	mWindow = new BuildingWindow(rect, this);
}

BUILDINGTYPE IndustrialBuilding::get_building_type()
{
	return BUILDINGTYPE::INDUSTRIAL_BUILDING;
}
