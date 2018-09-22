#include "MouseItem.h"
#include "LayerHandler.h"
#include "ConfigFile.h"
#include "IndustrialBuilding.h"
#include "AoeTower.h"
#include "HomingTower.h"
#include "MouseHandler.h"

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
}

MouseItem::~MouseItem()
{
	
}

void MouseItem::render(SDL_Point mouse_position)
{
	SDL_Rect dest;
	dest.x = mouse_position.x - mSprite->get_width() / 2;
	dest.y = mouse_position.y - mSprite->get_height() / 2;
	dest.w = mSprite->get_width();
	dest.h = mSprite->get_height();
	
	gLayer_handler->render_to_layer(this->mSprite, LAYERS::OVERLAY, nullptr, &dest);
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
	if(mouse_x < 1250)
	{
		SDL_Point p;
		p.x = mouse_x - mSprite->get_width() / 2;
		p.y = mouse_y - mSprite->get_height() / 2;
		std::string kind_of_object = gConfig_file->Value(mName_of_object + "/menuitem", "kind_of_object");
		if (mLevel->get_ressources()->sub(&mConstruction_costs))
		{
			if (kind_of_object == "homingtower") { new HomingTower(this->mName_of_object, p, this->mLevel); }
			if (kind_of_object == "aoetower") { new AoeTower(this->mName_of_object, p, this->mLevel); }
			if (kind_of_object == "industrialbuilding") { new IndustrialBuilding(this->mName_of_object, p, mLevel); }
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


