#include "MenuItem.h"
#include "ConfigFile.h"
#include "SDL_setup.h"

MenuItem::MenuItem(std::string path_of_texture) 
{
	mSprite = gTextures->get_texture(gConfig_file->Value(path_of_texture, "path"));
	mCoords.x = 1400;
	mCoords.y = 100;
}

MenuItem::~MenuItem()
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





