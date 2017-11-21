#pragma once
#include <vector>
#include <SDL.h>
#include <map>
#include "LTexture.h"

enum LAYERS
{
	BACKGROUND,
	BUILDINGS,
	ENEMIES,
	SHOTS,
	OVERLAY,
	COUNT
};

struct 
{
public:

};

class LayerHandler
{
public:
	LayerHandler();
	~LayerHandler();

	//renders everything currently in mPairs in the right order to the screen
	void render_everything();

	//adds a texture to be shown on a specified layer in the next frame
	void render_to_layer(LTexture* tex, LAYERS layer);

private:

	//puts empty vectors into mPairs along with every Layer
	void init_mpairs();

	//holds the textures that are going to be shown on different layers
	std::map<LAYERS, std::vector<LTexture*>*> mPairs;

};  
          