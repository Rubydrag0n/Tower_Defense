#pragma once
#include <SDL.h>
#include "LTexture.h"

enum LButtonSprite
{
	BUTTON_SPRITE_MOUSE_OUT = 0,
	BUTTON_SPRITE_MOUSE_OVER_MOTION = 1,
	BUTTON_SPRITE_MOUSE_DOWN = 2,
	BUTTON_SPRITE_MOUSE_UP = 3,
	BUTTON_SPRITE_TOTAL = 4
};

class Button
{
public:
	Button();
	~Button();

	//Sets top left position
	void setPosition(int x, int y);

	//Sets width and height
	void setDimension(int w, int h);

	//Sets the Clips
	void setSpriteClips(SDL_Rect *clips);

	//Handles mouse event
	void handleEvent(SDL_Event* e);

	//Shows button sprite
	void render();

private:
	//Top left position
	SDL_Point mPosition;

	//Width and height
	int mWidth;
	int mHeight;

	//The Position of the Clips
	SDL_Rect mSpriteClips[BUTTON_SPRITE_TOTAL];

	//Texture of the Button
	LTexture mButtonSpriteSheetTexture;

	//Currently used global sprite
	LButtonSprite mCurrentSprite;
};
