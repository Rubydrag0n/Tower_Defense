#include "WareHouse.h"
#include "Menu.h"
#include "Production.h"

Warehouse::Warehouse(std::string logistics_building_name, SDL_Point coords, Level *level, LAYERS click_layer, LAYERS render_layer) : 
	Building{std::move(logistics_building_name), coords, level, click_layer, render_layer}
{
	//since this is a warehouse it needs to "consume" everything so it requests everything from producing buildings
	for (auto i = 0; i < RESOURCES_TOTAL; i++) {
		(*mProducing)[RESOURCETYPES(i)] = CONSUMING;
	}
}

void Warehouse::on_tick()
{
	update_building_window();
	Building::on_tick();
}

void Warehouse::render()
{
	Building::render();
	//Warehouse Window is always shown
	mBuilding_window->set_rendering_enabled(true);
}

BUILDINGTYPE Warehouse::get_building_type()
{
	return WAREHOUSE;
}

void Warehouse::update_building_window()
{
	Building::update_building_window();
}

