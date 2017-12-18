#pragma once
#include "LTexture.h"
#include "CoordinatesInDouble.h"

class Particles
{
public:
	Particles::Particles(std::string particle_name);
	Particles::~Particles();

	void update();


private:
	LTexture* mTexture;

	//how many ticks have gone by since the creation of this particle
	int mTick;
	//how many ticks this particle will live
	int mLife_ticks;
	//how fast the particle is rotating in deg/tick
	float mRotation_speed;
	CoordinatesInDouble mPosition;
	CoordinatesInDouble mSpeed_and_direction;
	SDL_RendererFlip mFlip;
};
