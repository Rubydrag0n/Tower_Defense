#include "Window.h"
#include "SDL_setup.h"
#include "ConfigFile.h"
#include "LayerHandler.h"


Window::Window(const SDL_Rect dim, const STYLE style) : mDim(dim), mStyle(style), mR(0), mG(0), mB(0)
{
	this->set_clickable_space(dim);
	const auto category = "frames/" + std::to_string(int(style));
	mBottom_left_corner = gTextures->get_texture(gConfig_file->value(category, "blcpath"));
	mBottom_right_corner = gTextures->get_texture(gConfig_file->value(category, "brcpath"));
	mTop_left_corner = gTextures->get_texture(gConfig_file->value(category, "tlcpath"));
	mTop_right_corner = gTextures->get_texture(gConfig_file->value(category, "trcpath"));
	mHorizontal_border = gTextures->get_texture(gConfig_file->value(category, "hbpath"));
	mVertical_border = gTextures->get_texture(gConfig_file->value(category, "vbpath"));
	mBackground = gTextures->get_texture(gConfig_file->value(category, "bgcolorpath"));

	mCorner_height = gConfig_file->value(category, "corner_height");
	mCorner_width = gConfig_file->value(category, "corner_width");
	mBorder_thickness = gConfig_file->value(category, "border_thickness");
}

Window::~Window() = default;

void Window::render()
{
	//draw the inner color of the window (background of the window)
	SDL_Rect dest;
	dest.w = mDim.w;
	dest.h = mDim.h;

	dest.x = mDim.x;
	dest.y = mDim.y;

	gLayer_handler->render_to_layer(mBackground, LAYERS::WINDOWS, nullptr, &dest);

	//draw the four corners
	gLayer_handler->render_to_layer(mTop_left_corner, LAYERS::WINDOWS, nullptr, &dest);
	dest.x = mDim.x + mDim.w - mCorner_width;
	gLayer_handler->render_to_layer(mTop_right_corner, LAYERS::WINDOWS, nullptr, &dest);
	dest.y = mDim.y + mDim.h - mCorner_height;
	gLayer_handler->render_to_layer(mBottom_right_corner, LAYERS::WINDOWS, nullptr, &dest);
	dest.x = mDim.x;
	gLayer_handler->render_to_layer(mBottom_left_corner, LAYERS::WINDOWS, nullptr, &dest);

	//draw the borders
	//horizontal top:
	dest.y = mDim.y;
	dest.w = 1;
	dest.h = mBorder_thickness;
	for (dest.x = mDim.x + mCorner_width; dest.x < mDim.x + mDim.w - mCorner_width; dest.x++)
	{
		gLayer_handler->render_to_layer(mHorizontal_border, LAYERS::WINDOWS, nullptr, &dest);
	}
	//horizontal bottom:
	dest.y = mDim.y + mDim.h - mBorder_thickness;
	for (dest.x = mDim.x + mCorner_width; dest.x < mDim.x + mDim.w - mCorner_width; dest.x++)
	{
		gLayer_handler->render_to_layer(mHorizontal_border, LAYERS::WINDOWS, nullptr, &dest);
	}
	//vertical left:
	dest.x = mDim.x;
	dest.w = mBorder_thickness;
	dest.h = 1;
	for (dest.y = mDim.y + mCorner_height; dest.y < mDim.y + mDim.h - mCorner_height; dest.y++)
	{
		gLayer_handler->render_to_layer(mVertical_border, LAYERS::WINDOWS, nullptr, &dest);
	}
	//vertical right
	dest.x = mDim.x + mDim.w - mBorder_thickness;
	for (dest.y = mDim.y + mCorner_height; dest.y < mDim.y + mDim.h - mCorner_height; dest.y++)
	{
		gLayer_handler->render_to_layer(mVertical_border, LAYERS::WINDOWS, nullptr, &dest);
	}
}

void Window::set_dim(const SDL_Rect dim)
{
	mDim = dim;
}

SDL_Rect Window::get_dim() const
{
	return mDim;
}
