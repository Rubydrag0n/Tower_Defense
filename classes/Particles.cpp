#include "Particles.h"
#include "ConfigFile.h"
#include "SDL_setup.h"
#include "LayerHandler.h"

Particle::Particle(std::string particle_name, CoordinatesInDouble pos, CoordinatesInDouble dir, float rot, float rot_speed)
	: mClips{}, mRotation_speed{ rot_speed }, mCurrent_rotation{ rot }, mPosition { pos }, mSpeed_and_direction{ dir }
{
	auto section = "animation/" + particle_name;

	mTexture = gTextures->get_texture(gConfig_file->Value(section, "path"));
	std::string fliptext = gConfig_file->Value(section, "flip");
	
	if (fliptext == "none") mFlip = SDL_FLIP_NONE;
	else if (fliptext == "vertical") mFlip = SDL_FLIP_VERTICAL;
	else if (fliptext == "horizontal") mFlip = SDL_FLIP_HORIZONTAL;
	else mFlip = SDL_FLIP_NONE;

	//initialize the clips
	int clip_width = gConfig_file->Value(section, "clip_width");
	int clip_height = gConfig_file->Value(section, "clip_height");
	int frames_total = gConfig_file->Value(section, "frames_total");
	int frames_horizontal = mTexture->get_width() / clip_width;
	int frames_vertical = mTexture->get_height() / clip_height;

	for (auto i = 0; i < mTexture->get_height(); i += clip_height)
	{
		for (auto j = 0; j < mTexture->get_width(); j += clip_width)
		{
			SDL_Rect clip;
			clip.x = j;
			clip.y = i;
			clip.w = clip_width;
			clip.h = clip_height;
			mClips.push_back(clip);
			// break if 
			if (frames_total - 1 >= i*frames_vertical + j) {
				break;
			}
		}
	}
	mTick = 0;
	mTickcount_per_clip = gConfig_file->Value(section, "tickcount_per_clip");
	mLife_ticks = mTickcount_per_clip * mClips.size();
}

Particle::~Particle()
{
	mClips.clear();
}

void Particle::render()
{
	update_animation();

	SDL_Rect dest;

	dest.x = mPosition.x - mCurrent_clip.w / 2;
	dest.y = mPosition.y - mCurrent_clip.h / 2;
	dest.w = mCurrent_clip.w;
	dest.h = mCurrent_clip.h;

	SDL_Point center;
	center.x = mCurrent_clip.w / 2;
	center.y = mCurrent_clip.h / 2;
	gLayer_handler->renderex_to_layer(this->mTexture, LAYERS::PARTICLES, &this->mCurrent_clip, &dest, this->mCurrent_rotation, &center, this->mFlip);
}

void Particle::on_tick()
{
	//if lifetime is over -> suicide
	if (mTick >= mLife_ticks - 1)
	{
		delete this;
	}
}

void Particle::update_animation()
{
	//rotate
	this->mCurrent_rotation += this->mRotation_speed;
	if (this->mCurrent_rotation >= 360.0) this->mCurrent_rotation -= 360;
	if (this->mCurrent_rotation < 0.0) this->mCurrent_rotation += 360;

	//move
	this->mPosition = this->mSpeed_and_direction + this->mPosition;

	this->mTick++;
	if (this->mTick >= this->mLife_ticks)
	{
		this->mTick--;
	}
	auto index = this->mTick / this->mTickcount_per_clip;
	this->mCurrent_clip = this->mClips.at(index);
}
