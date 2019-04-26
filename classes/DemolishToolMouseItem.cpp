#include "DemolishToolMouseItem.h"
#include "SDL_setup.h"
#include "Building.h"

DemolishToolMouseItem::DemolishToolMouseItem(const std::string& name_of_object, LTexture* sprite, Level* level, LAYERS click_layer) : MouseItem(name_of_object, sprite, level, click_layer)
{
	
}

void DemolishToolMouseItem::on_click(int mouse_x, int mouse_y)
{
	if (mouse_x < 1280)
	{
		SDL_Point p;
		const auto tile_x = mouse_x / TILE_WIDTH;
		const auto tile_y = mouse_y / TILE_HEIGHT;
		auto building = mLevel->get_building_matrix(tile_x, tile_y);
		if(building != nullptr)
		{
			building->demolish();
			delete building;
		}
	}
}

void DemolishToolMouseItem::on_mouse_over(int mouse_x, int mouse_y)
{
	if (mouse_x < 1280 && this->get_state() == MOUSE_DOWN_LEFT)
	{
		const auto tile_x = mouse_x / TILE_WIDTH;
		const auto tile_y = mouse_y / TILE_HEIGHT;
		auto building = mLevel->get_building_matrix(tile_x, tile_y);
		if (building != nullptr)
		{
			building->demolish();
			delete building;
		}
	}
}
