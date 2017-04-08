#include "Map.h"
#include "ConfigFile.h"
#include "SDL_setup.h"

Map::Map(char *map_path) : mMap_texture()
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
	update_map_texture();
}

Map::~Map()
{
	delete mMap;
	SDL_DestroyTexture(mMap_texture);
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
	SDL_RenderCopy(gRenderer, mMap_texture, nullptr, nullptr);
}

void Map::update_map_texture()
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
	mMap_texture = SDL_CreateTextureFromSurface(gRenderer, s);
	SDL_SetTextureBlendMode(mMap_texture, SDL_BLENDMODE_BLEND);
	SDL_FreeSurface(s);
}
