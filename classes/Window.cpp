#include "Window.h"
#include "SDL_setup.h"
#include "ConfigFile.h"
#include "LayerHandler.h"
#include "MouseHandler.h"


Window::Window(const SDL_Rect dim, LAYERS click_layer, LAYERS render_layer, const STYLE style) : Clickable(click_layer), Renderable(render_layer), mDim(dim), mStyle(style), mR(0), mG(0), mB(0)
{
	
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

	set_clickable_space(mDim);

}

void Window::render()
{
	//draw the inner color of the window (background of the window)
	SDL_Rect dest;
	dest.x = mDim.x;
	dest.y = mDim.y;
	dest.w = mDim.w;
	dest.h = mDim.h;

	gLayer_handler->render_to_layer(mBackground, mRender_layer, nullptr, &dest);

	dest.w = 0;	//don't scale corners
	dest.h = 0;

	//draw the four corners
	gLayer_handler->render_to_layer(mTop_left_corner, mRender_layer, nullptr, &dest);
	dest.x = mDim.x + mDim.w - mCorner_width;
	gLayer_handler->render_to_layer(mTop_right_corner, mRender_layer, nullptr, &dest);
	dest.y = mDim.y + mDim.h - mCorner_height;
	gLayer_handler->render_to_layer(mBottom_right_corner, mRender_layer, nullptr, &dest);
	dest.x = mDim.x;
	gLayer_handler->render_to_layer(mBottom_left_corner, mRender_layer, nullptr, &dest);

	//draw the borders
	//horizontal top:
	dest.x = mDim.x + mCorner_width;
	dest.y = mDim.y;
	dest.w = mDim.w - 2 * mCorner_width;
	dest.h = mBorder_thickness;

	gLayer_handler->render_to_layer(mHorizontal_border, mRender_layer, nullptr, &dest);

	//horizontal bottom:
	dest.y = mDim.y + mDim.h - mBorder_thickness;
	//width, height and x stay the same

	gLayer_handler->render_to_layer(mHorizontal_border, mRender_layer, nullptr, &dest);

	//vertical left:
	dest.x = mDim.x;
	dest.y = mDim.y + mCorner_height;
	dest.w = mBorder_thickness;
	dest.h = mDim.h - 2 * mCorner_height;

	gLayer_handler->render_to_layer(mVertical_border, mRender_layer, nullptr, &dest);

	//vertical right
	dest.x = mDim.x + mDim.w - mBorder_thickness;
	//width, height and y stay the same

	gLayer_handler->render_to_layer(mVertical_border, mRender_layer, nullptr, &dest);
}

void Window::set_dim(const SDL_Rect dim)
{
	mDim = dim;
}

SDL_Rect Window::get_dim() const
{
	return mDim;
}

void Window::add_text_to_window(Text* text)
{
	mTexts.push_back(text);
}

void Window::add_button_to_window(Button* button)
{
	mButtons.push_back(button);
}

