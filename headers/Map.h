#pragma once
#include "LTexture.h"
#include "Renderable.h"
#include <vector>
#include <map>

class Map : Renderable
{
public:
	//for the editor, loads an empty map
	Map();
	explicit Map(std::string map_path);
	~Map();

	int get_height() const;
	int get_width() const;

	//reads a map from a map file
	//returns true on success and false otherwise
	bool deserialize(std::string& path);

	void render() override;

protected:
	void update_map_texture() const;

	int mOffset_left;
	int mOffset_top;
	int mWidth;
	int mHeight;
	int mLayer_count;

	//members that are deserialized from the map file
	int mTile_size_x;
	int mTile_size_y;

	//a map containing the characters and their graphics
	std::map<char, LTexture*> mTiles;

	//a vector storing the paths to the tile textures so they can be deleted in the destructor
	std::vector<std::string> mTile_paths;
	
	//the finished texture, consisting of the backgroundlayer with added decorations
	LTexture *mMap_texture;
	LTexture *mBackground_texture;
};
