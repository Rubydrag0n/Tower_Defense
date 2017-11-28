#pragma once
#include "Textures.h"
#include "LTexture.h"

enum class STYLE
{
	STANDARD,
	COUNT
};

class Window
{
public:
	Window(SDL_Rect dim, STYLE style = STYLE::STANDARD);
	~Window();

	void render() const;

	void set_dim(SDL_Rect dim);

private:
	//the dimensions of the window
	SDL_Rect mDim;

	//the style of the window
	STYLE mStyle;

	//pointers to all the textures used
	LTexture *mBlcorner;
	LTexture *mBrcorner;
	LTexture *mTlcorner;
	LTexture *mTrcorner;
	LTexture *mHorizontalborder;
	LTexture *mVerticalborder;

	int mCorner_width;
	int mCorner_height;
	int mBorder_thickness;

	int mR;
	int mG;
	int mB;
};