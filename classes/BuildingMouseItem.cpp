#include "MouseItem.h"
#include "LayerHandler.h"
#include "ConfigFile.h"
#include "IndustrialBuilding.h"
#include "AoeTower.h"
#include "HomingTower.h"
#include "MouseHandler.h"
#include "SDL_setup.h"
#include "WareHouse.h"
#include "Path.h"
#include "BuildingMouseItem.h"

BuildingMouseItem::BuildingMouseItem(const std::string& name_of_object, LTexture* sprite, Level* level, const Resources& construction_costs, LAYERS click_layer) : MouseItem(name_of_object, sprite, level, click_layer)
{
	mConstruction_costs = construction_costs;
	int i = gConfig_file->value(name_of_object + "/stats", "tile");
	mTile_to_build_on = static_cast<TILETYPES>(i);
}

void BuildingMouseItem::render()
{
	MouseItem::render();
}

void BuildingMouseItem::on_click(const int mouse_x, const int mouse_y)
{
	if (mouse_x < 1280)
	{
		SDL_Point p;
		const auto grid_offset_x = mouse_x % TILE_WIDTH;
		const auto grid_offset_y = mouse_y % TILE_HEIGHT;
		p.x = mouse_x - grid_offset_x;
		p.y = mouse_y - grid_offset_y;
		const auto tile_x = mouse_x / TILE_WIDTH;
		const auto tile_y = mouse_y / TILE_HEIGHT;
		const std::string kind_of_object = gConfig_file->value(mName_of_object + "/menuitem", "kind_of_object");
		const auto tile_type = mLevel->get_map_matrix()[tile_x][tile_y];
		if (tile_type == mTile_to_build_on && mLevel->get_building_matrix(tile_x, tile_y) == nullptr)
		{
			//don't combine these with && since we only want to sub resources when we can actually build
			if (mLevel->get_resources()->sub(&mConstruction_costs))
			{
				if (kind_of_object == "homingtower") new HomingTower(this->mName_of_object, p, this->mLevel, BUILDINGS, BUILDINGS);
				if (kind_of_object == "aoetower") new AoeTower(this->mName_of_object, p, this->mLevel, BUILDINGS, BUILDINGS);
				if (kind_of_object == "industrialbuilding") new IndustrialBuilding(this->mName_of_object, p, mLevel, LAYERS::BUILDINGS, BUILDINGS);
				if (kind_of_object == "warehouse") new Warehouse(this->mName_of_object, p, mLevel, BUILDINGS, BUILDINGS);
				if (kind_of_object == "path") new Path(this->mName_of_object, p, mLevel, BUILDINGS, BUILDINGS);
			}
		}
	}
}

void BuildingMouseItem::on_mouse_over(const int mouse_x, const int mouse_y)
{
	if (mouse_x < 1280 && this->get_state() == MOUSE_DOWN_LEFT)
	{
		SDL_Point p;
		const auto grid_offset_x = mouse_x % TILE_WIDTH;
		const auto grid_offset_y = mouse_y % TILE_HEIGHT;
		p.x = mouse_x - grid_offset_x;
		p.y = mouse_y - grid_offset_y;
		const auto tile_x = mouse_x / 64;
		const auto tile_y = mouse_y / 64;
		const std::string kind_of_object = gConfig_file->value(mName_of_object + "/menuitem", "kind_of_object");
		const auto tile_type = mLevel->get_map_matrix()[tile_x][tile_y];
		if (tile_type == mTile_to_build_on && mLevel->get_building_matrix(tile_x, tile_y) == nullptr)
		{
			if (kind_of_object == "path")
			{
				if (mLevel->get_resources()->sub(&mConstruction_costs))
				{
					new Path(this->mName_of_object, p, mLevel, LAYERS::BUILDINGS, BUILDINGS);
				}
			}
		}
	}
}
