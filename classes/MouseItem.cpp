#include "MouseItem.h"
#include "LayerHandler.h"
#include "ConfigFile.h"
#include "IndustrialBuilding.h"
#include "AoeTower.h"
#include "HomingTower.h"
#include "MouseHandler.h"
#include "SDL_setup.h"

MouseItem::MouseItem(std::string name_of_object, LTexture* sprite, Level* level, Resources construction_costs)
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
	mGrid_sprite_path = std::string(gConfig_file->Value("grid", "path"));
	mGrid_sprite = gTextures->get_texture(mGrid_sprite_path);
	int i = gConfig_file->Value(name_of_object + "/stats", "tile");
	mTile_to_build_on = static_cast<TILETYPES>(i);
}

MouseItem::~MouseItem()
{
	
}

void MouseItem::render()
{
	SDL_Rect dest;
	int mouse_x, mouse_y;
	//get coordinates
	gMouse_handler->get_mouse_position(&mouse_x, &mouse_y);
	auto grid_offset_x = 0, grid_offset_y = 0;
	if(mouse_x < 1280)
	{
		grid_offset_x = (mouse_x) % TILE_WIDTH;
		grid_offset_y = (mouse_y) % TILE_HEIGHT;
	}

	dest.x = mouse_x - grid_offset_x;
	dest.y = mouse_y - grid_offset_y;
	dest.w = mSprite->get_width();
	dest.h = mSprite->get_height();

	gLayer_handler->render_to_layer(this->mSprite, LAYERS::WINDOWS, nullptr, &dest);
	
	dest.x = 0;
	dest.y = 0;
	dest.w = 1280;
	dest.h = 1024;
	gLayer_handler->render_to_layer(this->mGrid_sprite, LAYERS::BUILDINGS, nullptr, &dest);
}


std::string MouseItem::get_name_of_object()
{
	return this->mName_of_object;
}

void MouseItem::set_name_of_object(std::string name)
{
	mName_of_object = name;
}

void MouseItem::on_click(int mouse_x, int mouse_y)
{
	if(mouse_x < 1280)
	{
		SDL_Point p;
		auto grid_offset_x = (mouse_x) % TILE_WIDTH;
		auto grid_offset_y = (mouse_y) % TILE_HEIGHT;
		p.x = mouse_x - grid_offset_x;
		p.y = mouse_y - grid_offset_y;
		auto tile_x = mouse_x / 64;
		auto tile_y = mouse_y / 64;
		std::string kind_of_object = gConfig_file->Value(mName_of_object + "/menuitem", "kind_of_object");
		auto tiletype = mLevel->get_map_matrix()[tile_x][tile_y];
		if(tiletype == mTile_to_build_on)
		{
			if (mLevel->get_resources()->sub(&mConstruction_costs))
			{
				if (kind_of_object == "homingtower") { new HomingTower(this->mName_of_object, p, this->mLevel); }
				if (kind_of_object == "aoetower") { new AoeTower(this->mName_of_object, p, this->mLevel); }
				if (kind_of_object == "industrialbuilding") { new IndustrialBuilding(this->mName_of_object, p, mLevel); }
				mLevel->set_map_matrix(tile_x, tile_y, TILETYPES::BUILDING);
			}
		}
	}
}

void MouseItem::on_middle_click(int mouse_x, int mouse_y)
{
	
}

void MouseItem::on_mouse_over(int mouse_x, int mouse_y)
{
	
}

void MouseItem::on_right_click(int mouse_x, int mouse_y)
{
	gMouse_handler->set_item_on_mouse(nullptr);
}


