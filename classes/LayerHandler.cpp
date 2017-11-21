#include <LayerHandler.h>
#include <SDL.h>
#include "SDL_setup.h"

LayerHandler::LayerHandler() : mPairs{}
{
	this->init_mpairs();
}

LayerHandler::~LayerHandler()
{
	//loop through the pair
	for (auto const& x : mPairs)
	{
		//delete the vectors
		delete x.second;
		//don't delete the textures, they have nothing to do with that
	}
	//empty the map
	mPairs.clear();
}

void LayerHandler::render_to_layer(LTexture* tex, LAYERS layer)
{
	//insert the texture into the vector at the right layer
	this->mPairs.at(layer)->push_back(tex);
}

void LayerHandler::render_everything()
{
	/*for (auto const& x : mPairs)
	{
		x.second.at(0)->render()
	}*/
}

void LayerHandler::init_mpairs()
{
	for (auto i = 0; i < LAYERS::COUNT; i++)
	{
		auto v = new std::vector<LTexture*>();
		this->mPairs.insert(std::pair<LAYERS, std::vector<LTexture*>*>(LAYERS(i), v));
	}
}
