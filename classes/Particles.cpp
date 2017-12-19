#include "Particles.h"
#include "ConfigFile.h"
#include "SDL_setup.h"
#include "LayerHandler.h"

Particles::Particles(std::string particle_name, CoordinatesInDouble pos, CoordinatesInDouble dir, float rot) : mClips{}, mRotation_speed{rot}, mPosition{pos}, mSpeed_and_direction{dir}
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
		}
	}
	mTick = 0;
	mTickcount_per_clip = gConfig_file->Value(section, "tickcount_per_clip");
	mLife_ticks = mTickcount_per_clip * mClips.size();
}

Particles::~Particles()
{
	mClips.clear();
}

void Particles::render()
{
	update_animation_clip();

	SDL_Rect dest;

	dest.x = mPosition.x - mCurrent_clip.w / 2;
	dest.y = mPosition.y - mCurrent_clip.h / 2;
	dest.w = mCurrent_clip.w;
	dest.h = mCurrent_clip.h;

	gLayer_handler->render_to_layer(this->mTexture, LAYERS::ENEMIES, &this->mCurrent_clip, &dest);
}

bool Particles::delete_me() const
{
	if (mTick >= mLife_ticks)
	{
		return true;
	}
	return false;
}

void Particles::update_animation_clip()
{
	mTick++;
	if (mTick > mLife_ticks)
	{
		mTick--;
	}
	auto index = mTick / mTickcount_per_clip;
	mCurrent_clip = mClips.at(index);
}
