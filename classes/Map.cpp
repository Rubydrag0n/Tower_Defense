#include "Map.h"
#include "ConfigFile.h"
#include "SDL_setup.h"

Map::Map(char *map_path)
{
	ConfigFile cf("config/game.cfg");
	mOffset_left = cf.Value("map", "offset_left");
	mOffset_top = cf.Value("map", "offset_top");
	mWidth = cf.Value("map", "width");
	mHeight = cf.Value("map", "height");
	mLayer_count = cf.Value("map", "layer_count");
	mMap = new SDLMappy;
	if (mMap->LoadMap(map_path, mOffset_left, mOffset_top, mWidth, mHeight) == -1)
		printf("Could not load %s\n", map_path);
}

Map::~Map()
{
	delete mMap;
}

int Map::get_height() const
{
	return mHeight;
}

int Map::get_width() const
{
	return mWidth;
}

void Map::render() const
{
	mMap->MapMoveTo(mOffset_left, mOffset_top);
	mMap->MapChangeLayer(0);	//Background first
	auto s = SDL_LoadBMP("resources/background.bmp");
	if (mMap->MapDrawBG(s) == -1)
		printf("Failed to draw background of Map!\n");
	
	for (auto i = 1; i < mLayer_count; i++)
	{
		auto s2 = SDL_LoadBMP("resources/background.bmp");
		mMap->MapChangeLayer(i);
		if (mMap->MapDrawBGT(s2) == -1)
			printf("Failed to draw layer %i of foreground of Map!\n", mLayer_count);
		else
		{
			SDL_SetColorKey(s2, 1, 0);		//setting the transparent color for this surface to black (the 0)
			SDL_SetSurfaceBlendMode(s2, SDL_BLENDMODE_BLEND);
			SDL_BlitSurface(s2, nullptr, s, nullptr);		//blit the new layer on top of the total
			SDL_FreeSurface(s2);
		}
	}
	auto texture = SDL_CreateTextureFromSurface(gRenderer, s);

	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
	SDL_RenderCopy(gRenderer, texture, nullptr, nullptr);
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(s);
}
