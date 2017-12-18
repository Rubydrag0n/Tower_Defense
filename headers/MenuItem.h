#pragma once
#include "Clickable.h"
#include "Building.h"

class MenuItem :
	public Clickable
{
public:
	MenuItem();
	~MenuItem();


private:
	SDL_Point mCoords; //coordinates, where the item is placed in the menu
	std::string mPath_of_texture;
};
