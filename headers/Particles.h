#pragma once
#include "LTexture.h"
#include "CoordinatesInDouble.h"
#include <vector>

class Particles
{
public:
	Particles::Particles(std::string particle_name, CoordinatesInDouble pos, CoordinatesInDouble dir, float rot);
	Particles::~Particles();

	void render();
	bool delete_me() const;

private:
	void update_animation_clip();

	LTexture* mTexture;
	//stores all the rectangles of the different clips
	std::vector<SDL_Rect> mClips;
	//the clip that is currently shown
	SDL_Rect mCurrent_clip;
	//how many ticks have gone by since the creation of this particle
	int mTick;
	//how many ticks this particle will live
	int mLife_ticks;
	//how many ticks each clip is shown
	int mTickcount_per_clip;
	//how fast the particle is rotating in deg/tick
	float mRotation_speed;
	CoordinatesInDouble mPosition;
	CoordinatesInDouble mSpeed_and_direction;
	SDL_RendererFlip mFlip;
};
