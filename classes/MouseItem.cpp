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

MouseItem::MouseItem(const std::string& name_of_object, LTexture* sprite, Level* level, const Resources& construction_costs, LAYERS click_layer) : Clickable(click_layer)
{
	this->mName_of_object = name_of_object;
	SDL_Rect clickable;
	clickable.x = 0;
	clickable.y = 0;
	clickable.w = 1920;
	clickable.h = 1050;
	this->set_clickable_space(clickable);
	mSprite = sprite;
	mLevel = level;
	mConstruction_costs = construction_costs;
	mGrid_sprite_path = std::string(gConfig_file->value("grid", "path"));
	mGrid_sprite = gTextures->get_texture(mGrid_sprite_path);
	int i = gConfig_file->value(name_of_object + "/stats", "tile");
	mTile_to_build_on = static_cast<TILETYPES>(i);
}

void MouseItem::render() const
{
	SDL_Rect dest;
	int mouse_x;
	int mouse_y;
	//get coordinates
	gMouse_handler->get_mouse_position(&mouse_x, &mouse_y);
	auto grid_offset_x = 0;
	auto grid_offset_y = 0;
	if(mouse_x < 1280)
	{
		grid_offset_x = mouse_x % TILE_WIDTH;
		grid_offset_y = mouse_y % TILE_HEIGHT;
	}

	dest.x = mouse_x - grid_offset_x;
	dest.y = mouse_y - grid_offset_y;
	dest.w = mSprite->get_width();
	dest.h = mSprite->get_height();

	gLayer_handler->render_to_layer(this->mSprite, mClick_layer, nullptr, &dest);
	
	dest.x = 0;
	dest.y = 0;
	dest.w = 1280;
	dest.h = 1024;
	gLayer_handler->render_to_layer(this->mGrid_sprite, LAYERS::BUILDINGS, nullptr, &dest);
}


std::string MouseItem::get_name_of_object() const
{
	return this->mName_of_object;
}

void MouseItem::set_name_of_object(const std::string& name)
{
	mName_of_object = name;
}

void MouseItem::on_click(const int mouse_x, const int mouse_y)
{
	if(mouse_x < 1280)
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
		if(tile_type == mTile_to_build_on)
		{
			//don't combine these with && since we only want to sub resources when we can actually build
			if (mLevel->get_resources()->sub(&mConstruction_costs))
			{
				if (kind_of_object == "homingtower") new HomingTower(this->mName_of_object, p, this->mLevel, BUILDINGS, BUILDINGS);
				if (kind_of_object == "aoetower") new AoeTower(this->mName_of_object, p, this->mLevel, BUILDINGS, BUILDINGS);
				if (kind_of_object == "industrialbuilding") new IndustrialBuilding(this->mName_of_object, p, mLevel, LAYERS::BUILDINGS, BUILDINGS);
				if (kind_of_object == "warehouse") new Warehouse(this->mName_of_object, p, mLevel, BUILDINGS, BUILDINGS);
				if (kind_of_object == "path") new Path(this->mName_of_object, p, mLevel, BUILDINGS, BUILDINGS);
				mLevel->set_map_matrix(tile_x, tile_y, BUILDINGTILE);
			}
		}
	}
}

void MouseItem::on_right_click(int mouse_x, int mouse_y)
{
	gMouse_handler->set_item_on_mouse(nullptr);
}

void MouseItem::on_mouse_over(const int mouse_x, const int mouse_y)
{
	if (mouse_x < 1280 && this->get_state() == MOUSE_DOWN_LEFT)
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
		if (tile_type == mTile_to_build_on)
		{
			if (kind_of_object == "path")
			{
				if (mLevel->get_resources()->sub(&mConstruction_costs))
				{
					new Path(this->mName_of_object, p, mLevel, LAYERS::BUILDINGS, BUILDINGS);
					mLevel->set_map_matrix(tile_x, tile_y, BUILDINGTILE);
				}
			}
		}
	}
}
