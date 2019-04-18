#include "Text.h"
#include "LayerHandler.h"

Text::Text(std::string text, SDL_Rect dim, LAYERS layer, SDL_Color text_color, Renderable* texture_to_render_on) : mText(text), mDim(dim), mLayer(layer), mTexture_to_render_on(texture_to_render_on)
{
	mText_texture->load_from_rendered_text(text, text_color);
	mText_color = text_color;
}

Text::~Text()
{
	mText_texture->free();
	delete mText_texture;
}

void Text::render()
{
	//Show current button sprite
	if (mTexture_to_render_on != nullptr)
	{
		if (mTexture_to_render_on->is_rendering_enabled())
		{
			gLayer_handler->render_to_layer(mText_texture, mLayer, nullptr, &mDim);
		}
	}
	else
	{
		gLayer_handler->render_to_layer(mText_texture, mLayer, nullptr, &mDim);
	}
}

void Text::add_y_dim(int y)
{
	mDim.y += y;
}

void Text::add_x_dim(int x)
{
	mDim.x += x;
}

void Text::set_text(std::string text)
{
	mText = text;
	mText_texture->load_from_rendered_text(text, mText_color);
}


