#include "IndustrialBuilding.h"
#include "ConfigFile.h"
#include "SDL_setup.h"
#include "Carriage.h"

IndustrialBuilding::IndustrialBuilding(std::string industrial_building_name, const SDL_Point coords, Level* level, LAYERS click_layer, LAYERS render_layer) : Building(
	std::move(industrial_building_name), coords, level, click_layer, render_layer)
{
	SDL_Rect rect;
	rect.x = mCoords.x + mSprite_dimensions.w;
	rect.y = mCoords.y - 200;
	rect.w = 200;
	rect.h = 600;
	mWindow = new BuildingWindow(rect, this);
	mWindow->set_rendering_enabled(false);

	mCarriage = new Carriage("carriage", mLevel, ENEMIES, this, reinterpret_cast<Building*>(mLevel->get_main_building()));
}

BUILDINGTYPE IndustrialBuilding::get_building_type()
{
	return INDUSTRIAL_BUILDING;
}
