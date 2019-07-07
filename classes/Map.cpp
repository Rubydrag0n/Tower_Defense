#include "Map.h"
#include "ConfigFile.h"
#include "LayerHandler.h"
#include <fstream>
#include <sstream>
#include "SDL_setup.h"
#include <SDL.h>

Map::Map(std::string map_path): Renderable(BACKGROUND)
{
	mTile_size_x = 0;
	mTile_size_y = 0;

	mOffset_left = gConfig_file->value("map", "offset_left");
	mOffset_top = gConfig_file->value("map", "offset_top");
	mWidth = gConfig_file->value("map", "width");
	mHeight = gConfig_file->value("map", "height");
	mLayer_count = gConfig_file->value("map", "layer_count");

	mBackground_texture = gTextures->get_texture("resources/background.bmp");

	mMap = new SDLMappy;

	/*if (mMap->load_map(map_path, mOffset_left, mOffset_top, mWidth, mHeight) == -1)
		printf("Could not load %s\n", map_path);*/
	mMap_texture = new LTexture();

	this->deserialize(map_path);
}

Map::~Map()
{
	delete mMap;
	mMap_texture->free();
}

int Map::get_height() const
{
	return mHeight;
}

int Map::get_width() const
{
	return mWidth;
}

bool Map::deserialize(std::string& path)
{
	const auto tiles_x = 20;
	const auto tiles_y = 16;

	std::ifstream file(path);
	if (!file.is_open())
	{
		return false;
	}

	std::string content;

	//reading tile sizes
	std::getline(file, content);
	this->mTile_size_x = std::stoi(content);
	std::getline(file, content);
	this->mTile_size_y = std::stoi(content);

	//reading the tiles
	std::getline(file, content);
	while (content != "endtiles")
	{
		char tile_char;
		std::string tile_path;
		std::stringstream ss;
		ss << content;
		ss >> tile_char;
		ss >> tile_path;

		//TODO: Error handling
		auto tile_texture = gTextures->get_texture(tile_path);

		mTiles.insert(std::pair<char, LTexture*>(tile_char, tile_texture));

		std::getline(file, content);
	}

   	const auto map_width = mTile_size_x * tiles_x;
	const auto map_height = mTile_size_y * tiles_y;

	if (!mMap_texture->create_blank(map_width, map_height, SDL_TEXTUREACCESS_TARGET))
	{
		printf("Failed to create target texture!\n");
		file.close();
		return false;
	}

	//we want to render to the texture
	mMap_texture->set_as_render_target();

	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(gRenderer);

	SDL_Rect dest;
	dest.x = 0;
	dest.y = 0;
	dest.w = mTile_size_x;
	dest.h = mTile_size_y;

	//read the background and render it to an LTexture
	for (auto y = 0; y < tiles_y; ++y)
	{
		std::getline(file, content);
		char tile;
		std::stringstream ss;
		ss << content;
		
		for (auto x = 0; x < tiles_x; ++x)
		{
			ss >> tile;
			mTiles[tile]->render(&dest);
			dest.x += dest.w;
		}

		dest.x = 0;
		dest.y += dest.h;
	}

	return true;
}

void Map::render()
{
	gLayer_handler->render_to_layer(mBackground_texture, this->get_render_layer(), nullptr, nullptr);
	
	gLayer_handler->render_to_layer(mMap_texture, this->get_render_layer(), nullptr, nullptr);
}

void Map::update_map_texture() const
{
	//old code using old implementation of SDL_Mappy, isn't used anymore

	mMap->map_move_to(mOffset_left, mOffset_top);
	mMap->map_change_layer(0);	//Background first
	const auto s = SDL_LoadBMP("resources/background.bmp");
	if (mMap->map_draw_bg(s) == -1)
		printf("Failed to draw background of Map!\n");
	
	for (auto i = 1; i < mLayer_count; i++)
	{
		const auto s2 = SDL_LoadBMP("resources/background.bmp");
		mMap->map_change_layer(i);
		if (mMap->map_draw_bgt(s2) == -1)
			printf("Failed to draw layer %i of foreground of Map!\n", mLayer_count);
		else
		{
			SDL_SetColorKey(s2, 1, 0);		//setting the transparent color for this surface to black (the 0)
			SDL_SetSurfaceBlendMode(s2, SDL_BLENDMODE_BLEND);
			SDL_BlitSurface(s2, nullptr, s, nullptr);		//blit the new layer on top of the total
			SDL_FreeSurface(s2);
		}
	}
	mMap_texture->load_from_surface(s);
	mMap_texture->set_blend_mode(SDL_BLENDMODE_BLEND);
	SDL_FreeSurface(s);
}
