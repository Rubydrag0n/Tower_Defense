#pragma once
#include <SDL.h>

#include "Textures.h"
#include "Clickable.h"
#include "Renderable.h"
#include "Enums.h"

/*
 * This class implements a window of given dimensions
 */
class Window : public Clickable, public Renderable
{
public:
	explicit Window(SDL_Rect dim, STYLE style = STYLE::STANDARD);
	~Window();

	void render() override;

	void set_dim(SDL_Rect dim);
	SDL_Rect get_dim() const;

private:
	//the dimensions of the window
	SDL_Rect mDim;

	//the style of the window
	STYLE mStyle;

	//pointers to all the textures used
	LTexture *mBottom_left_corner;
	LTexture *mBottom_right_corner;
	LTexture *mTop_left_corner;
	LTexture *mTop_right_corner;
	LTexture *mHorizontal_border;
	LTexture *mVertical_border;
	LTexture *mBackground;

	int mCorner_width;
	int mCorner_height;
	int mBorder_thickness;

	int mR;
	int mG;
	int mB;
};