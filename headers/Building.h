#pragma once
#include <string>
#include <SDL.h>

class Building
{
public:
	Building(std::string building_name);
	~Building();

	void render();

protected:
	SDL_Point mPosition;
	SDL_Texture *mSprite;
	SDL_Rect mSprite_dimensions;
	SDL_Rect mClip;
};
