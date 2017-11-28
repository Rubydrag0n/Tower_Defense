#include "Window.h"
#include "SDL_setup.h"
#include "ConfigFile.h"

Window::Window(SDL_Rect dim, STYLE style)
{
	ConfigFile cf("config/game.cfg");
	mDim = dim;
	mStyle = style;

	std::string category = "frames/" + std::to_string(int(style));
	mBlcorner = gTextures->get_texture(cf.Value(category, "blcpath"));
	mBrcorner = gTextures->get_texture(cf.Value(category, "brcpath"));
	mTlcorner = gTextures->get_texture(cf.Value(category, "tlcpath"));
	mTrcorner = gTextures->get_texture(cf.Value(category, "trcpath"));
	mHorizontalborder = gTextures->get_texture(cf.Value(category, "hbpath"));
	mVerticalborder = gTextures->get_texture(cf.Value(category, "vbpath"));

	mCorner_height = cf.Value(category, "corner_height");
	mCorner_width = cf.Value(category, "corner_width");
	mBorder_thickness = cf.Value(category, "border_thickness");

	mR = cf.Value(category, "backgroundcolorred");
	mG = cf.Value(category, "backgroundcolorgreen");
	mB = cf.Value(category, "backgroundcolorblue");
}

Window::~Window()
{
	//Don't destroy textures here
}

void Window::render() const
{
	//TODO: do this with LayerHandler...
	//draw the inner color of the window (background of the window)
	SDL_Rect dest;
	dest.w = mDim.w;
	dest.h = mDim.h;

	dest.x = mDim.x;
	dest.y = mDim.y;

	SDL_SetRenderDrawColor(gRenderer, mR, mG, mB, 255);
	SDL_RenderFillRect(gRenderer, &dest);

	//draw the four corners
	mTlcorner->render(dest.x, dest.y);
	dest.x = mDim.x + mDim.w - mCorner_width;
	mTrcorner->render(dest.x, dest.y);
	dest.y = mDim.y + mDim.h - mCorner_height;
	mBrcorner->render(dest.x, dest.y);
	dest.x = mDim.x;
	mBlcorner->render(dest.x, dest.y);

	//draw the borders
	//horizontal top:
	dest.y = mDim.y;
	dest.w = 1;
	dest.h = mBorder_thickness;
	for (dest.x = mDim.x + mCorner_width; dest.x < mDim.x + mDim.w - mCorner_width; dest.x++)
	{
		mHorizontalborder->render(dest.x, dest.y);
	}
	//horizontal bottom:
	dest.y = mDim.y + mDim.h - mBorder_thickness;
	for (dest.x = mDim.x + mCorner_width; dest.x < mDim.x + mDim.w - mCorner_width; dest.x++)
	{
		mHorizontalborder->render(dest.x, dest.y);
	}
	//vertical left:
	dest.x = mDim.x;
	dest.w = mBorder_thickness;
	dest.h = 1;
	for (dest.y = mDim.y + mCorner_height; dest.y < mDim.y + mDim.h - mCorner_height; dest.y++)
	{
		mVerticalborder->render(dest.x, dest.y);
	}
	//vertical right
	dest.x = mDim.x + mDim.w - mBorder_thickness;
	for (dest.y = mDim.y + mCorner_height; dest.y < mDim.y + mDim.h - mCorner_height; dest.y++)
	{
		mVerticalborder->render(dest.x, dest.y);
	}
}

void Window::set_dim(SDL_Rect dim)
{
	this->mDim = dim;
}
