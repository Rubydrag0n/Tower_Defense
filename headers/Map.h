#pragma once
#include "SDLMappy.h"
#include <SDL.h>

class Map
{
public:
	Map(char *map_path);
	~Map();

	int get_height() const;
	int get_width() const;

	void render() const;

private:
	void update_map_texture();

	SDL_Texture *mMap_texture;

	SDLMappy *mMap;
	int mOffset_left;
	int mOffset_top;
	int mWidth;
	int mHeight;
	int mLayer_count;
};

