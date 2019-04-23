#pragma once
#include <SDL.h>

#include "Textures.h"
#include "Clickable.h"
#include "Renderable.h"
#include "Enums.h"

/*
 * This class implements a window of given dimensions
 */
class Window : public Renderable, public Clickable
{
public:
	explicit Window(SDL_Rect dim, LAYERS click_layer, LAYERS render_layer, STYLE style = STYLE::STANDARD);

	void render() override;
	void on_click(int mouse_x, int mouse_y) override;

	void set_dim(SDL_Rect dim);
	SDL_Rect get_dim() const;

private:
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

protected:
	//the dimensions of the window
	SDL_Rect mDim;
};