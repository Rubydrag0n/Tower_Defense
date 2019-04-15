#include "IndustrialBuilding.h"
#include "ConfigFile.h"
#include "SDL_setup.h"
#include "Carriage.h"

IndustrialBuilding::IndustrialBuilding(std::string industrial_building_name, const SDL_Point coords, Level* level) : Building(
	std::move(industrial_building_name), coords, level)
{
	SDL_Rect rect;
	rect.x = mCoords.x;
	rect.y = mCoords.y - 200;
	rect.w = 200;
	rect.h = 200;
	mWindow = new BuildingWindow(rect, this);

	mCarriage = new Carriage("carriage", mLevel, this, reinterpret_cast<Building*>(mLevel->get_main_building()));
}

BUILDINGTYPE IndustrialBuilding::get_building_type()
{
	return INDUSTRIAL_BUILDING;
}
