#include "Button.h"

Button::Button() : mSpriteClips{}, mButtonSpriteSheetTexture()
{
	mPosition.x = 0;
	mPosition.y = 0;
	mWidth = 0;
	mHeight = 0;

	mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;

}

Button::~Button()
{
}

void Button::set_position(int x, int y)
{
	mPosition.x = x;
	mPosition.y = y;
}

void Button::set_dimension(int w, int h)
{
	mWidth = w;
	mHeight = h;
}

void Button::set_sprite_clips(SDL_Rect * clips)
{
	for (auto i = 0; i < LButtonSprite::BUTTON_SPRITE_TOTAL; i++)
	{
		mSpriteClips[i] = clips[i];
	}
}

void Button::handle_event(SDL_Event* e)
{
	//If mouse event happened
	if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP)
	{
		//Get mouse position
		int x, y;
		SDL_GetMouseState(&x, &y);

		//Check if mouse is in button
		auto inside = true;

		//Mouse is left of the button
		if (x < mPosition.x)
		{
			inside = false;
		}
		//Mouse is right of the button
		else if (x > mPosition.x + mWidth)
		{
			inside = false;
		}
		//Mouse above the button
		else if (y < mPosition.y)
		{
			inside = false;
		}
		//Mouse below the button
		else if (y > mPosition.y + mHeight)
		{
			inside = false;
		}

		//Mouse is outside button
		if (!inside)
		{
			mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
		}
		//Mouse is inside button
		else
		{
			//Set mouse over sprite
			switch (e->type)
			{
			case SDL_MOUSEMOTION:
				mCurrentSprite = BUTTON_SPRITE_MOUSE_OVER_MOTION;
				break;

			case SDL_MOUSEBUTTONDOWN:
				mCurrentSprite = BUTTON_SPRITE_MOUSE_DOWN;
				break;

			case SDL_MOUSEBUTTONUP:
				mCurrentSprite = BUTTON_SPRITE_MOUSE_UP;
				break;
			default:
				break;
			}
		}
	}
}

void Button::render()
{
	//Show current button sprite
	mButtonSpriteSheetTexture.render(mPosition.x, mPosition.y, &mSpriteClips[mCurrentSprite]);
}
