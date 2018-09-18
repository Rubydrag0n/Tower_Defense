#pragma once
#include <vector>
#include "LTexture.h"
#include "CoordinatesInDouble.h"
#include "Renderable.h"
#include "Entity.h"

class Particle :
	public Entity
{
public:
	Particle::Particle(std::string particle_name, CoordinatesInDouble pos, CoordinatesInDouble dir, float rot, float rot_speed);
	Particle::~Particle();

	void render() override;
	void on_tick() override;

private:
	void update_animation();

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
	//current rotation of the particle
	float mCurrent_rotation;
	//how fast the particle is rotating in deg/tick
	float mRotation_speed;
	//current position of the particle (may change in its lifetime)
	CoordinatesInDouble mPosition;
	//speed and direction of the particle
	CoordinatesInDouble mSpeed_and_direction;
	//if the particle is flipped (horizontal or vertical) or not
	SDL_RendererFlip mFlip;
};
