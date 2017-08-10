#include "Building.h"
#include "ConfigFile.h"
#include "SDL_setup.h"


Building::Building(std::string building_name)
{
	ConfigFile cf("config/game.cfg");
	mSprite = loadTexture(cf.Value(building_name + "/sprite", "path"));
	mSprite_dimensions.w = cf.Value(building_name + "/sprite", "image_width");
	mSprite_dimensions.h = cf.Value(building_name + "/sprite", "image_height");
	 

	mPosition.x = 0;
	mPosition.y = 0;

	SDL_SetTextureBlendMode(mSprite, SDL_BLENDMODE_BLEND);

}


Building::~Building()
{
	SDL_DestroyTexture(mSprite);
}

void Building::render()
{
	SDL_Rect dest;
	dest.x = mPosition.x;
	dest.y = mPosition.y;
	SDL_RenderCopy(gRenderer, mSprite, nullptr, &dest);
}
