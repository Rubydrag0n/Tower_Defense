#include "Map.h"
#include "ConfigFile.h"

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

void Map::render(SDL_Surface* s) const
{
	mMap->MapMoveTo(mOffset_left, mOffset_top);
	if (mMap->MapDrawBG(s) == -1)
		printf("Failed to draw background of Map!\n");

	/*for (auto i = 1; i < mLayer_count; i++)
	{
		mMap->MapChangeLayer(i);
		if (mMap->MapDrawFG(s) == -1)
			printf("Failed to draw layer %i of foreground of Map!\n", mLayer_count);
	}*/
}
