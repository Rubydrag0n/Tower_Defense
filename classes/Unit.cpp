#include "Unit.h"
#include "Textures.h"
#include "ConfigFile.h"
#include "SDL_setup.h"


Unit::Unit(std::string unit_name) : mDefense(), mClips(), mSprite_dimensions()
{
	ConfigFile cf("config/game.cfg");
	//load path of the sprite
	std::string path = cf.Value(unit_name + "/sprite", "path");
	mSprite = gTextures->get_texture(path);	//set the pointer to the sprite

	mSprite_dimensions.w = cf.Value(unit_name + "/sprite", "image_width");
	mSprite_dimensions.h = cf.Value(unit_name + "/sprite", "image_height");
	mSprite_dimensions.x = 0;
	mSprite_dimensions.y = 0;
	int clip_width = cf.Value(unit_name + "/sprite", "clip_width");
	int clip_height = cf.Value(unit_name + "/sprite", "clip_height");
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
	mTickcount_per_clip = cf.Value(unit_name + "/sprite", "tickcount_per_clip");
	
	// animation tick count is how many ticks it takes for one cycle of the animation to run through.
	// each clip gets shown for tickcount_per_clip ticks
	mAnimation_tick_count = mSprite_dimensions.w / clip_width * mTickcount_per_clip;

	//gets initialized in the derived classes
	mPosition.x = 0;
	mPosition.y = 0;
	mDirection = DIRECTION::DOWN;

	mMoveSpeed = cf.Value(unit_name + "/stats", "movementspeed");
	mDefense.set_defenses(double(int(cf.Value(unit_name + "/stats", "health"))),
						  double(int(cf.Value(unit_name + "/stats", "armor"))),
						  double(int(cf.Value(unit_name + "/stats", "magicres"))),
						  double(int(cf.Value(unit_name + "/stats", "fireres"))),
						  double(int(cf.Value(unit_name + "/stats", "waterres"))),
						  double(int(cf.Value(unit_name + "/stats", "elecres"))));
	mDefense.set_immunities(bool(int(cf.Value(unit_name + "/stats", "physimm"))),
							bool(int(cf.Value(unit_name + "/stats", "magicimm"))),
							bool(int(cf.Value(unit_name + "/stats", "fireimm"))),
							bool(int(cf.Value(unit_name + "/stats", "waterimm"))),
							bool(int(cf.Value(unit_name + "/stats", "elecimm"))));
	//setting up the blending
	SDL_SetTextureBlendMode(mSprite, SDL_BLENDMODE_BLEND);

	update_animation_clip();
}

Unit::~Unit()
{
	//Don't destroy texture here, that is handled by the Textures class
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
