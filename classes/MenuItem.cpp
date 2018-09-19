#include "MenuItem.h"
#include "ConfigFile.h"
#include "SDL_setup.h"
#include "LayerHandler.h"
#include "EntityHandler.h"
#include "Tower.h"
#include "HomingTower.h"
#include "AoeTower.h"
#include "IndustrialBuilding.h"
#include "Window.h"
#include "MouseHandler.h"

MenuItem::MenuItem(std::string name_of_object, Level *level) 
{
	mName_of_object = name_of_object;
	mKind_of_object.assign(gConfig_file->Value(name_of_object + "/menuitem", "kind_of_object"));
	mSprite = gTextures->get_texture(gConfig_file->Value(name_of_object + "/sprite", "path"));
	mCoords.x = gConfig_file->Value(mName_of_object + "/menuitem", "x");
	mCoords.y = gConfig_file->Value(mName_of_object + "/menuitem", "y");
	mLevel = level;
	SDL_Rect clickable;
	clickable.x = mCoords.x;
	clickable.y = mCoords.y;
	clickable.w = mSprite->get_width();
	clickable.h = mSprite->get_height();
	this->set_clickable_space(clickable);
	this->set_state(MOUSE_OUT);

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
	dest.x = this->get_coords().x;
	dest.y = this->get_coords().y;
	dest.w = this->get_sprite()->get_width();
	dest.h = this->get_sprite()->get_height();
	gLayer_handler->render_to_layer(this->get_sprite(), LAYERS::BACKGROUND, nullptr, &dest);
	if (this->get_state() == LClickableState::MOUSE_DOWN_LEFT && this == gMouse_handler->get_item_on_mouse())
	{
		gMouse_handler->render(dest, mouse_position, this->get_sprite());
	}
}

void MenuItem::on_click(int mouse_x, int mouse_y)
{
	SDL_Rect clickable;
	if(this->get_state() == LClickableState::MOUSE_DOWN_LEFT && this == gMouse_handler->get_item_on_mouse())
	{
		SDL_Point p;
		p.x = mouse_x - get_sprite()->get_width() / 2;
		p.y = mouse_y - get_sprite()->get_height() / 2;

		if (mLevel->get_ressources()->sub(&mConstruction_costs))
		{
			if (this->mKind_of_object == "homingtower") { new HomingTower(this->mName_of_object, p, this->mLevel); }
			if (this->mKind_of_object == "aoetower") { new AoeTower(this->mName_of_object, p, this->mLevel); }
			if (this->mKind_of_object == "industrialbuilding") { new IndustrialBuilding(this->mName_of_object, p, mLevel); }
			clickable.x = mCoords.x;
			clickable.y = mCoords.y;
			clickable.w = mSprite->get_width();
			clickable.h = mSprite->get_height();
			this->set_clickable_space(clickable);
			this->set_state(MOUSE_OUT);
		}
		

	}
	else if(this->get_state() == LClickableState::MOUSE_OUT)
	{
		gMouse_handler->set_item_on_mouse(this);		
		this->set_state(MOUSE_DOWN_LEFT);
		clickable.x = 0;
		clickable.y = 0;
		clickable.w = 1250;
		clickable.h = 1050;
		this->set_clickable_space(clickable);
	}
}

void MenuItem::on_middle_click(int mouse_x, int mouse_y)
{

}

void MenuItem::on_mouse_over(int mouse_x, int mouse_y)
{

}

void MenuItem::on_right_click(int mouse_x, int mouse_y)
{
	if(this->get_state() == LClickableState::MOUSE_DOWN_LEFT)
	{
		SDL_Rect clickable;
		clickable.x = mCoords.x;
		clickable.y = mCoords.y;
		clickable.w = mSprite->get_width();
		clickable.h = mSprite->get_height();
		this->set_clickable_space(clickable);
		this->set_state(MOUSE_OUT);
	}
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
