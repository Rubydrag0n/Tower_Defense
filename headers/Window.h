#pragma once
#include"Textures.h"

enum STYLE
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
	SDL_Texture *mBlcorner;
	SDL_Texture *mBrcorner;
	SDL_Texture *mTlcorner;
	SDL_Texture *mTrcorner;
	SDL_Texture *mHorizontalborder;
	SDL_Texture *mVerticalborder;

	int mCorner_width;
	int mCorner_height;
	int mBorder_thickness;

	int mR;
	int mG;
	int mB;
};