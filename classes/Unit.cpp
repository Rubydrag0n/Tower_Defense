#include "Unit.h"
#include "ConfigFile.h"
#include "SDL_setup.h"


Unit::Unit(SDL_Point pos, std::string cfg_path) : mDefense(), mSprite(), mClips(), mSprite_dimensions()
{
	ConfigFile cf(cfg_path);
	mSprite = loadTexture(cf.Value("sprite", "path"));

	mSprite_dimensions.w = cf.Value("sprite", "image_width");
	mSprite_dimensions.h = cf.Value("sprite", "image_height");
	int clip_width = cf.Value("sprite", "clip_width");
	int clip_height = cf.Value("sprite", "clip_height");
	for (auto i = 0; i < mSprite_dimensions.h; i += clip_height)
	{
		for (auto j = 0; j < mSprite_dimensions.w; j += clip_width)
		{
			SDL_Rect clip;
			clip.x = j;
			clip.y = i;
			clip.w = clip_width;
			clip.h = clip_height;
			mClips.push_back(clip);
		}
	}
	mAnimation_tick = 0;
	mTickcount_per_clip = cf.Value("sprite", "tickcount_per_clip");
	
	// animation tick count is how many ticks it takes for one cycle of the animation to run through.
	// each clip gets shown for tickcount_per_clip ticks
	mAnimation_tick_count = mSprite_dimensions.w / clip_width * mTickcount_per_clip;

	mPosition = pos;
	mDirection = DIRECTION::DOWN;

	mMoveSpeed = cf.Value("stats", "movementspeed");
	mDefense.set_defenses(double(int(cf.Value("stats", "health"))), 
						  double(int(cf.Value("stats", "armor"))), 
						  double(int(cf.Value("stats", "magicres"))), 
						  double(int(cf.Value("stats", "fireres"))), 
						  double(int(cf.Value("stats", "waterres"))), 
						  double(int(cf.Value("stats", "elecres"))));
	mDefense.set_immunities(bool(int(cf.Value("stats", "physimm"))), 
							bool(int(cf.Value("stats", "magicimm"))), 
							bool(int(cf.Value("stats", "fireimm"))), 
							bool(int(cf.Value("stats", "waterimm"))), 
							bool(int(cf.Value("stats", "elecimm"))));
	//setting up the blending
	SDL_SetTextureBlendMode(mSprite, SDL_BLENDMODE_BLEND);

	update_animation_clip();
}

Unit::~Unit()
{
	SDL_DestroyTexture(mSprite);
}

void Unit::render()
{
	update_animation_clip();

	SDL_Rect dest;
	dest.x = mPosition.x - mCurrent_clip.w / 2;
	dest.y = mPosition.y - mCurrent_clip.h / 2;
	dest.w = mCurrent_clip.w;
	dest.h = mCurrent_clip.h;

	SDL_RenderCopy(gRenderer, mSprite, &mCurrent_clip, &dest);
}

void Unit::update_animation_clip()
{
	mAnimation_tick++;
	if (mAnimation_tick >= mAnimation_tick_count)
	{
		mAnimation_tick = 0;
	}
	auto index = mAnimation_tick / mTickcount_per_clip;
	index += mAnimation_tick_count / mTickcount_per_clip * mDirection;
	mCurrent_clip = mClips.at(index);
}
