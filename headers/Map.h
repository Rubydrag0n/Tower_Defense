#pragma once
#include "SDLMappy.h"
#include "LTexture.h"

class Map
{
public:
	explicit Map(char *map_path);
	~Map();

	int get_height() const;
	int get_width() const;

	void render() const;

private:
	void update_map_texture() const;

	LTexture *mMap_texture;

	SDLMappy *mMap;
	int mOffset_left;
	int mOffset_top;
	int mWidth;
	int mHeight;
	int mLayer_count;
};
