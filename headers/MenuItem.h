#pragma once
#include "Clickable.h"
#include "Building.h"

class MenuItem 
{
public:
	MenuItem(std::string path_of_texture);
	~MenuItem();

	LTexture* get_sprite();
	SDL_Point get_coords();


private:
	SDL_Point mCoords; //coordinates, where the item is placed in the menu
	LTexture* mSprite;
};
