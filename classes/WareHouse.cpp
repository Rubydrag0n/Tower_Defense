#include "WareHouse.h"
#include "Menu.h"

Warehouse::Warehouse(std::string logistics_building_name, SDL_Point coords, Level *level) : 
	Building{logistics_building_name, coords, level}
{

}

Warehouse::~Warehouse()
{

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
