#pragma once
#include"Textures.h"
#include <SDL.h>
#include "Clickable.h"
#include "Renderable.h"

enum STYLE
{
	STANDARD,
	COUNT
};

/*
 * This class implements a window of given dimensions
 */
class Window : public Clickable, public Renderable
{
public:
	Window(SDL_Rect dim, STYLE style = STYLE::STANDARD);
	~Window();

	virtual void render();

	void set_dim(SDL_Rect dim);
	SDL_Rect get_dim();

	void on_click(int mouse_x, int mouse_y) override;
	void on_mouse_over(int mouse_x, int mouse_y) override;
	void on_right_click(int mouse_x, int mouse_y) override;
	void on_middle_click(int mouse_x, int mouse_y) override;

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
	LTexture *mBackground;

	int mCorner_width;
	int mCorner_height;
	int mBorder_thickness;

	int mR;
	int mG;
	int mB;
};