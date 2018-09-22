#include "MenuItem.h"
#include "ConfigFile.h"
#include "SDL_setup.h"
#include "LayerHandler.h"
#include "Tower.h"
#include "IndustrialBuilding.h"
#include "MouseHandler.h"

MenuItem::MenuItem(std::string name_of_object, Level *level, SDL_Point coords) 
{
	mName_of_object = name_of_object;
	mSprite = gTextures->get_texture(gConfig_file->Value(name_of_object + "/sprite", "path"));
	//mCoords.x = gConfig_file->Value(mName_of_object + "/menuitem", "x");
	//mCoords.y = gConfig_file->Value(mName_of_object + "/menuitem", "y");
	mCoords = coords;
	mLevel = level;
	SDL_Rect clickable;
	clickable.x = mCoords.x;
	clickable.y = mCoords.y;
	clickable.w = mSprite->get_width();
	clickable.h = mSprite->get_height();
	this->set_clickable_space(clickable);

	//set the construction costs of the building
	mConstruction_costs.set_resources(gConfig_file->Value(name_of_object + "/stats", "goldcosts"),
		gConfig_file->Value(name_of_object + "/stats", "woodcosts"),
		gConfig_file->Value(name_of_object + "/stats", "stonecosts"),
		gConfig_file->Value(name_of_object + "/stats", "ironcosts"),
		gConfig_file->Value(name_of_object + "/stats", "energycosts"),
		gConfig_file->Value(name_of_object + "/stats", "watercosts"),
		gConfig_file->Value(name_of_object + "/stats", "foodcosts"));
}

MenuItem::~MenuItem()
{
	
}

void MenuItem::render(SDL_Point mouse_position)
{
	SDL_Rect dest;
	dest.x = mCoords.x;
	dest.y = mCoords.y;
	dest.w = mSprite->get_width();
	dest.h = mSprite->get_height();
	this->set_clickable_space(dest);
	gLayer_handler->render_to_layer(mSprite, LAYERS::BACKGROUND, nullptr, &dest);
	if(gMouse_handler->get_item_on_mouse() != nullptr)
	{
		if (this->mName_of_object == gMouse_handler->get_item_on_mouse()->get_name_of_object())
		{
			gMouse_handler->get_item_on_mouse()->render(mouse_position);
		}
	}
}

void MenuItem::delete_clickable_space()
{
	SDL_Rect clickable;
	clickable.x = 0;
	clickable.y = 0;
	clickable.h = 0;
	clickable.w = 0;
	this->set_clickable_space(clickable);
}


void MenuItem::on_click(int mouse_x, int mouse_y)
{
	gMouse_handler->set_item_on_mouse(new MouseItem(mName_of_object, mSprite, mLevel, mConstruction_costs));
}

void MenuItem::on_middle_click(int mouse_x, int mouse_y)
{

}

void MenuItem::on_mouse_over(int mouse_x, int mouse_y)
{

}

void MenuItem::on_right_click(int mouse_x, int mouse_y)
{

}


LTexture* MenuItem::get_sprite()
{
	return this->mSprite;
}

SDL_Point MenuItem::get_coords()
{
	return this->mCoords;
}

Resources MenuItem::get_construction_costs() const
{
	return mConstruction_costs;
}
