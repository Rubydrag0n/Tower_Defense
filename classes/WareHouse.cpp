#include "WareHouse.h"
#include "Menu.h"
#include "Production.h"

Warehouse::Warehouse(std::string logistics_building_name, SDL_Point coords, Level *level, LAYERS click_layer, LAYERS render_layer) : 
	Building{std::move(logistics_building_name), coords, level, click_layer, render_layer}
{
	SDL_Rect rect;
	rect.x = mCoords.x;
	rect.y = mCoords.y - 200;
	rect.w = 200;
	rect.h = 200;
	mWindow = new BuildingWindow(rect, this);
	mWindow->set_rendering_enabled(false);

	//since this is a warehouse it needs to "consume" everything so it requests everything from producing buildings
	for (auto i = 0; i < RESOURCES_TOTAL; i++) {
		(*mProducing)[RESOURCETYPES(i)] = CONSUMING;
	}
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
	return WAREHOUSE;
}
