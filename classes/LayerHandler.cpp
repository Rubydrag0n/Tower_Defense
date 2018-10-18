#include <LayerHandler.h>
#include <SDL.h>
#include "SDL_setup.h"
#include <iostream>

LayerHandler* gLayer_handler = nullptr;

RenderTexture::RenderTexture(LTexture* texture, SDL_Rect* src_rect, SDL_Rect* dst_rect) : texture{ texture }, src_rect{}, dst_rect{},
																						  ex{false}, angle{ 0.0 }, center{}, flip{SDL_FLIP_NONE}
{
	//if no src rect is given, the whole texture is the src_rect
	if (src_rect == nullptr)
	{
		//if src_rect.x is negative one then on render it will just use a nullptr
		this->src_rect.x = -1;
	}
	else
	{
		this->src_rect = *src_rect;
	}
	//if no dst_rect is given, the whole screen is the dst_rect
	if (dst_rect == nullptr)
	{
		this->dst_rect.x = 0;
		this->dst_rect.y = 0;
	} 
	else
	{
		this->dst_rect = *dst_rect;
	}
}

RenderTexture::RenderTexture(LTexture* texture, SDL_Rect* src_rect, SDL_Rect* dst_rect, double angle, SDL_Point* center, SDL_RendererFlip flip) :
	texture{ texture }, src_rect{}, dst_rect{},
	ex{ true }, angle{ angle }, center{}, flip{ flip }
{
	//if no src rect is given, the whole texture is the src_rect
	if (src_rect == nullptr)
	{
		//if src_rect.x is negative one then on render it will just use a nullptr
		this->src_rect.x = -1;
	}
	else
	{
		this->src_rect = *src_rect;
	}
	//if no dst_rect is given, the whole screen is the dst_rect
	if (dst_rect == nullptr)
	{
		this->dst_rect.x = 0;
		this->dst_rect.y = 0;
	}
	else
	{
		this->dst_rect = *dst_rect;
	}
	if (center != nullptr) {
		this->center.x = center->x;
		this->center.y = center->y;
	} else
	{
		this->center.x = 0;
		this->center.y = 0;
	}
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
	SDL_RenderClear(gRenderer);
	for (auto i = 0; i < int(LAYERS::LAYERS_TOTAL); i++)
	{
		std::vector<RenderTexture*>* x = mPairs.at(LAYERS(i));

		for (auto j = 0; j < x->size(); j++) {
			auto r = x->at(j);

			SDL_Rect* src = nullptr;
			if (r->src_rect.x != -1)
			{
				src = &r->src_rect;
			}

			r->texture->render(r->dst_rect.x, r->dst_rect.y, src, r->angle, &r->center, r->flip);
			delete r;
		}
		x->clear();
	}
	SDL_RenderPresent(gRenderer);
}

void LayerHandler::init_mpairs()
{
	for (auto i = 0; i < int(LAYERS::LAYERS_TOTAL); i++)
	{
		auto v = new std::vector<RenderTexture*>();
		this->mPairs.insert(std::pair<LAYERS, std::vector<RenderTexture*>*>(LAYERS(i), v));
	}
}
