#pragma once
#include "Defense.h"
#include <SDL.h>
#include <vector>
#include "LTexture.h"
#include "Entity.h"
#include "CoordinatesInDouble.h"

//The numbers are the rowpositions of the rotations in the sprite image
enum DIRECTION
{
	DOWN = 4,
	LEFT = 5,
	UP = 6,
	RIGHT = 7
};
/*
 * A Unit
 * Can walk over the screen (with animation?)
 * Has defensive stats and movement speed
 */
class Unit : public Entity
{
public:
	Unit(std::string unit_name);
	~Unit();

	void render();

protected:
	void update_animation_clip();

	Defense mDefense;
	double mMoveSpeed;
	
	CoordinatesInDouble mPosition;
	//Only up, right, down or left for the choosing of the right sprite
	DIRECTION mDirection;

	int mAnimation_tick;
	int mAnimation_tick_count;
	int mTickcount_per_clip;
	LTexture *mSprite;
	std::vector<SDL_Rect> mClips;
	SDL_Rect mCurrent_clip;
	SDL_Rect mSprite_dimensions;
};
