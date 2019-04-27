#include "WareHouse.h"
#include "Menu.h"
#include "Production.h"

Warehouse::Warehouse(std::string logistics_building_name, SDL_Point coords, Level *level, LAYERS click_layer, LAYERS render_layer) : 
	Building{std::move(logistics_building_name), coords, level, click_layer, render_layer}
{
	SDL_Rect rect;
	rect.x = 1610;
	rect.y = 824;
	rect.w = 200;
	rect.h = 200;
	mWindow = new BuildingWindow(rect, this);
	mWindow->set_rendering_enabled(true);
	mWindow->enable();


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
	//Warehouse Window is always shown
	mWindow->set_rendering_enabled(true);
	mWindow->set_clicked(true);
}

BUILDINGTYPE Warehouse::get_building_type()
{
	return WAREHOUSE;
}
