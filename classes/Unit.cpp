#include "Unit.h"
#include "Textures.h"
#include "ConfigFile.h"
#include "SDL_setup.h"
#include "LayerHandler.h"


Unit::Unit(std::string unit_name) : mDefense(), mClips(), mSprite_dimensions()
{
	auto sprite_section = unit_name + "/sprite";
	auto stats_section = unit_name + "/stats";
	//load path of the sprite
	std::string path = gConfig_file->Value(sprite_section, "path");
	mSprite = gTextures->get_texture(path);	//set the pointer to the sprite

	mSprite_dimensions.w = mSprite->get_width();
	mSprite_dimensions.h = mSprite->get_height();
	mSprite_dimensions.x = 0;
	mSprite_dimensions.y = 0;
	int clip_width = gConfig_file->Value(sprite_section, "clip_width");
	int clip_height = gConfig_file->Value(sprite_section, "clip_height");
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
	mTickcount_per_clip = gConfig_file->Value(sprite_section, "tickcount_per_clip");
	
	// animation tick count is how many ticks it takes for one cycle of the animation to run through.
	// each clip gets shown for tickcount_per_clip ticks
	mAnimation_tick_count = mSprite_dimensions.w / clip_width * mTickcount_per_clip;

	//gets initialized in the derived classes
	mPosition.x = 0;
	mPosition.y = 0;
	mDirection = DIRECTION::DOWN;

	mMoveSpeed = gConfig_file->Value(stats_section, "movementspeed");
	mDefense.set_defenses(double(int(gConfig_file->Value(stats_section, "health"))),
						  double(int(gConfig_file->Value(stats_section, "armor"))),
						  double(int(gConfig_file->Value(stats_section, "magicres"))),
						  double(int(gConfig_file->Value(stats_section, "fireres"))),
						  double(int(gConfig_file->Value(stats_section, "waterres"))),
						  double(int(gConfig_file->Value(stats_section, "elecres"))));
	mDefense.set_immunities(bool(int(gConfig_file->Value(stats_section, "physimm"))),
							bool(int(gConfig_file->Value(stats_section, "magicimm"))),
							bool(int(gConfig_file->Value(stats_section, "fireimm"))),
							bool(int(gConfig_file->Value(stats_section, "waterimm"))),
							bool(int(gConfig_file->Value(stats_section, "elecimm"))));
	//setting up the blending
	mSprite->set_blend_mode(SDL_BLENDMODE_BLEND);

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

	gLayer_handler->render_to_layer(this->mSprite, LAYERS::ENEMIES, &this->mCurrent_clip, &dest);
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
