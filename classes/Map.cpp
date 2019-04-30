#include "Map.h"
#include "ConfigFile.h"
#include "LayerHandler.h"

Map::Map(char *map_path): Renderable(BACKGROUND)
{
	mOffset_left = gConfig_file->value("map", "offset_left");
	mOffset_top = gConfig_file->value("map", "offset_top");
	mWidth = gConfig_file->value("map", "width");
	mHeight = gConfig_file->value("map", "height");
	mLayer_count = gConfig_file->value("map", "layer_count");
	mMap = new SDLMappy;
	if (mMap->load_map(map_path, mOffset_left, mOffset_top, mWidth, mHeight) == -1)
		printf("Could not load %s\n", map_path);
	mMap_texture = new LTexture();
	update_map_texture();
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

void Map::render()
{
	gLayer_handler->render_to_layer(mMap_texture, this->get_render_layer(), nullptr, nullptr);
}

void Map::update_map_texture() const
{
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
