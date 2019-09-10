#include "IndustrialBuilding.h"
#include "ConfigFile.h"
#include "SDL_setup.h"
#include "Carriage.h"
#include "IndustrialBuildingWindow.h"

IndustrialBuilding::IndustrialBuilding(std::string industrial_building_name, const SDL_Point coords, Level* level, LAYERS click_layer, LAYERS render_layer) : Building(
	std::move(industrial_building_name), coords, level, click_layer, render_layer)
{
	mCarriage = new Carriage("carriage", mLevel, ENEMIES, this, reinterpret_cast<Building*>(mLevel->get_main_building()));
}

BUILDINGTYPE IndustrialBuilding::get_building_type()
{
	return INDUSTRIAL_BUILDING;
}

void IndustrialBuilding::on_click(int mouse_x, int mouse_y)
{
	SDL_Rect rect;
	rect.x = 1280;
	rect.y = 600;
	rect.w = 600;
	rect.h = 200;
	mLevel->get_menu()->set_building_window(new IndustrialBuildingWindow(rect, this));
	Building::on_click(mouse_x, mouse_y);
}

