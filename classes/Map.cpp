#include "Map.h"
#include "ConfigFile.h"
#include "LayerHandler.h"
#include "SDL_setup.h"
#include <SDL.h>
#include <google/protobuf/util/json_util.h>

#include <iostream>
#include <fstream>
#include <sstream>

Map::Map() : Renderable(BACKGROUND)
{
	mTile_size_x = 0;
	mTile_size_y = 0;

	mOffset_left = 0;
	mOffset_top = 0;
	mWidth = 0;
	mHeight = 0;
	mLayer_count = 0;

	mBackground_texture = gTextures->get_texture("resources/background.bmp");

	mMap_texture = new LTexture();
}


Map::Map(std::string map_path) : Renderable(BACKGROUND)
{
	mTile_size_x = 0;
	mTile_size_y = 0;

	mOffset_left = gConfig_file->value("map", "offset_left");
	mOffset_top = gConfig_file->value("map", "offset_top");
	mWidth = gConfig_file->value("map", "width");
	mHeight = gConfig_file->value("map", "height");
	mLayer_count = gConfig_file->value("map", "layer_count");

	mBackground_texture = gTextures->get_texture("resources/background.bmp");

	/*if (mMap->load_map(map_path, mOffset_left, mOffset_top, mWidth, mHeight) == -1)
		printf("Could not load %s\n", map_path);*/
	mMap_texture = new LTexture();

	this->deserialize(map_path);
}

Map::~Map()
{
	mMap_texture->free();
	delete mMap_texture;

	for (auto& pair : mMap.tile_map())
	{
		gTextures->del_texture(pair.second);
	}
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
	std::ifstream file(path);
	if (!file.is_open())
	{
		std::cerr << "Couldn't read map from " << path << "." << std::endl;
		return false;
	}

	std::stringstream json_string;

	json_string << file.rdbuf();

	auto status = google::protobuf::util::JsonStringToMessage(json_string.str(), &mMap);

	return status.ok();
}

void Map::render()
{
	gLayer_handler->render_to_layer(mBackground_texture, this->get_render_layer(), nullptr, nullptr);
	
	gLayer_handler->render_to_layer(mMap_texture, this->get_render_layer(), nullptr, nullptr);
}
