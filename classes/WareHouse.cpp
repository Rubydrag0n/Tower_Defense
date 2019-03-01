#include "WareHouse.h"
#include "Menu.h"

Warehouse::Warehouse(std::string logistics_building_name, SDL_Point coords, Level *level) : 
	Building{std::move(logistics_building_name), coords, level}
{
	SDL_Rect rect;
	rect.x = mCoords.x;
	rect.y = mCoords.y - 200;
	rect.w = 200;
	rect.h = 200;
	mWindow = new BuildingWindow(rect, this);
}

void Warehouse::on_tick()
{
	Building::on_tick();
}

void Warehouse::render()
{
	Building::render();
}

BUILDINGTYPE Warehouse::get_building_type()
{
	return BUILDINGTYPE::LOGISTICS_BUILDING;
}
