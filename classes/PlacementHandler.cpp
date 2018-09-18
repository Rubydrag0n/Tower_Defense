#include "PlacementHandler.h"
#include "Level.h"

PlacementHandler* gPlacement_handler = nullptr;

PlacementHandler::PlacementHandler(Level* level) : mHeld_object{}
{
	this->mLevel = level;
}

void PlacementHandler::on_click(int mouse_x, int mouse_y)
{
	SDL_Point p;
	p.x = mouse_x;
	p.y = mouse_y;
	if (mLevel->get_ressources()->sub(&mConstruction_costs))
	{
		if (this->mKind_of_object == "homingtower") { new HomingTower(this->mName_of_object, p, this->mLevel); }
		if (this->mKind_of_object == "aoetower") { new AoeTower(this->mName_of_object, p, this->mLevel); }
		if (this->mKind_of_object == "industrialbuilding") { new IndustrialBuilding(this->mName_of_object, p, mLevel); }
		mClickstate = CLICKSTATE::UNCLICKED;
	}

	clickable.x = mCoords.x;
	clickable.y = mCoords.y;
	clickable.w = mSprite->get_width();
	clickable.h = mSprite->get_height();
	this->set_clickable_space(clickable);
}

bool PlacementHandler::isHolding()
{
	return this->mHeld_object == "";
}

void PlacementHandler::set_level(Level * l)
{
	this->mLevel = l;
	this->set_held_object("");
}

void PlacementHandler::set_held_object(std::string object_name)
{
	this->mHeld_object = object_name;
}
