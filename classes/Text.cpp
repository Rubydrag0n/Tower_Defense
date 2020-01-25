#include "Text.h"
#include "LayerHandler.h"
#include <iomanip>

Text::Text(std::string text, SDL_Rect dim, LAYERS render_layer, SDL_Color text_color, Renderable* texture_to_render_on) : Renderable(render_layer), mText(text), mDim(dim), mTexture_to_render_on(texture_to_render_on)
{
	mText_color = text_color;
	mText_texture->load_from_rendered_text(text, mText_color);
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
			gLayer_handler->render_to_layer(mText_texture, mRender_layer, nullptr, &mDim);
		}
	}
	else
	{
		gLayer_handler->render_to_layer(mText_texture, mRender_layer, nullptr, &mDim);
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
	if(text != mText)
	{
		mText = text;
		mText_texture->load_from_rendered_text(text, mText_color);
	}
}

std::string Text::remove_trailing_zeros(std::string s)
{
	//erase all 0 at the end of the string, if a point is then at the end erase it too
	s.erase(s.find_last_not_of('0') + 1, std::string::npos);
	s.erase(s.find_last_not_of('.') + 1, std::string::npos);

	//set precision(number of digits after the point)
	//can probably be done better
	auto found_point = false;
	auto precision = 2;
	std::string result;
	for(auto i = 0; i< s.size(); i++)
	{
		if (!precision) break;
		if (found_point) precision--;
		if (s.at(i) == '.') found_point = true;
		result.push_back(s.at(i));
	}

	return result;
}



