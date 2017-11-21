#include <LayerHandler.h>
#include <SDL.h>
#include "SDL_setup.h"

RenderTexture::RenderTexture(LTexture* texture, SDL_Rect* src_rect, SDL_Rect* dst_rect) : texture{ texture }, src_rect{ src_rect }, dst_rect{ dst_rect }, 
																						  ex{false}, angle{ 0.0 }, center{nullptr}, flip{SDL_FLIP_NONE}
{

}

RenderTexture::RenderTexture(LTexture* texture, SDL_Rect* src_rect, SDL_Rect* dst_rect, double angle, SDL_Point* center, SDL_RendererFlip flip) :
	texture{ texture }, src_rect{ src_rect }, dst_rect{ dst_rect },
	ex{ true }, angle{ angle }, center{ center }, flip{ flip }
{
	
}


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

void LayerHandler::render_to_layer(LTexture* texture, LAYERS layer, SDL_Rect* src_rect, SDL_Rect* dst_rect)
{
	//create the RenderTexture object with all the necessary meta data
	auto render_texture = new RenderTexture(texture, src_rect, dst_rect);
	//insert the texture into the vector at the right layer
	this->mPairs.at(layer)->push_back(render_texture);
}

void LayerHandler::renderex_to_layer(LTexture* texture, LAYERS layer, SDL_Rect* src_rect, SDL_Rect* dst_rect, double angle, SDL_Point* center, SDL_RendererFlip flip)
{

	//create the RenderTexture object with all the necessary meta data
	auto render_texture = new RenderTexture(texture, src_rect, dst_rect, angle, center, flip);
	//insert the texture into the vector at the right layer
	this->mPairs.at(layer)->push_back(render_texture);
}

void LayerHandler::present()
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
		auto v = new std::vector<RenderTexture*>();
		this->mPairs.insert(std::pair<LAYERS, std::vector<RenderTexture*>*>(LAYERS(i), v));
	}
}
