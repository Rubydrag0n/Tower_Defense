#include "Unit.h"
#include "Textures.h"
#include "ConfigFile.h"
#include "SDL_setup.h"
#include "LayerHandler.h"
#include "Menu.h"
#include "Window.h"


Unit::Unit(const std::string& unit_name, Level* level, LAYERS render_layer) : Entity(render_layer), Clickable(render_layer), mLevel(level), mCenter(), mCurrent_clip(), mSprite_dimensions()
{
	const auto sprite_section = unit_name + "/sprite";
	const auto stats_section = unit_name + "/stats";
	//load path of the sprite
	std::string path = gConfig_file->value(sprite_section, "path");
	mSprite = gTextures->get_texture(path);	//set the pointer to the sprite

	mSprite_dimensions.w = mSprite->get_width();
	mSprite_dimensions.h = mSprite->get_height();
	//mSprite_dimensions.w = gConfig_file->value(sprite_section, "clip_width");
	//mSprite_dimensions.h = gConfig_file->value(sprite_section, "clip_height");
	mSprite_dimensions.x = 0;
	mSprite_dimensions.y = 0;
	int clip_width = gConfig_file->value(sprite_section, "clip_width");
	int clip_height = gConfig_file->value(sprite_section, "clip_height");
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
	mTickcount_per_clip = gConfig_file->value(sprite_section, "tickcount_per_clip");

	this->mCenter.x = gConfig_file->value(sprite_section, "rotation_center_x");
	this->mCenter.y = gConfig_file->value(sprite_section, "rotation_center_y");
	
	// animation tick count is how many ticks it takes for one cycle of the animation to run through.
	// each clip gets shown for tickcount_per_clip ticks
	mAnimation_tick_count = mSprite_dimensions.w / clip_width * mSprite_dimensions.h / clip_height * mTickcount_per_clip;

	//gets initialized in the derived classes
	mPosition.x = 0;
	mPosition.y = 0;
	mDirection = DIRECTION::DOWN;

	mMove_speed = gConfig_file->value(stats_section, "movementspeed");
	mDefense = new Defense();
	mDefense->set_health(double(int(gConfig_file->value_or_zero(stats_section, "health"))));
	mDefense->set_full_health(double(int(gConfig_file->value_or_zero(stats_section, "health"))));
	mDefense->set_resistances(double(int(gConfig_file->value_or_zero(stats_section, "armor"))),
						  double(int(gConfig_file->value_or_zero(stats_section, "magicres"))),
						  double(int(gConfig_file->value_or_zero(stats_section, "fireres"))),
						  double(int(gConfig_file->value_or_zero(stats_section, "waterres"))),
						  double(int(gConfig_file->value_or_zero(stats_section, "elecres"))));
	mDefense->set_immunities(bool(int(gConfig_file->value_or_zero(stats_section, "physimm"))),
							bool(int(gConfig_file->value_or_zero(stats_section, "magicimm"))),
							bool(int(gConfig_file->value_or_zero(stats_section, "fireimm"))),
							bool(int(gConfig_file->value_or_zero(stats_section, "waterimm"))),
							bool(int(gConfig_file->value_or_zero(stats_section, "elecimm"))));
	//setting up the blending
	mSprite->set_blend_mode(SDL_BLENDMODE_BLEND);

	update_animation_clip();

	mClickable_space.x = int(mPosition.x);
	mClickable_space.y = int(mPosition.y);
	mClickable_space.w = gConfig_file->value(sprite_section, "clip_width");
	mClickable_space.h = gConfig_file->value(sprite_section, "clip_height");

	SDL_Rect rect;
	rect.x = 1680;
	rect.y = 824;
	rect.w = 200;
	rect.h = 200;

	mUnit_window = new Window(rect, WINDOWS, WINDOWS);
	mDefense_values = new Text*[RESISTANCES_TOTAL];
	SDL_Color text_color = { 0,0,0,0 };

	SDL_Rect dest;
	dest.x = mUnit_window->get_dim().x + 20;
	dest.y = mUnit_window->get_dim().y + 20;
	dest.w = 0;
	dest.h = 0;
	auto health_name = new Text("Health", dest, WINDOWCONTENT, text_color, this);
	mUnit_window->add_text_to_window(health_name);
	mHealth_value = new Text(std::to_string(int(mDefense->get_full_health())), dest, WINDOWCONTENT, text_color, mUnit_window);
	mHealth_value->add_x_dim(100);
	for (auto i = 0; i < RESISTANCES_TOTAL; i++)
	{
		dest.y += 20;
		auto defense_names = new Text(Defense::get_name(RESISTANCES(i)), dest, WINDOWCONTENT, text_color, mUnit_window);
		mUnit_window->add_text_to_window(defense_names);
		mDefense_values[i] = new Text(std::to_string(int(mDefense->get_resistance(RESISTANCES(i)))), dest, WINDOWCONTENT, text_color, mUnit_window);
		mDefense_values[i]->add_x_dim(100);
		mUnit_window->add_text_to_window(mDefense_values[i]);
	}
	dest.y += 20;
	auto move_speed_name = new Text("Move Speed", dest, WINDOWCONTENT, text_color, mUnit_window);
	mUnit_window->add_text_to_window(move_speed_name);
	mMove_speed_value = new Text(std::to_string(int(mMove_speed)), dest, WINDOWCONTENT, text_color, this);
	mMove_speed_value->add_x_dim(100);
	mUnit_window->add_text_to_window(mMove_speed_value);
}

Unit::~Unit()
{
	delete mDefense;
}


void Unit::render()
{
	for (auto i = 0; i < RESISTANCES_TOTAL; i++)
	{
		mDefense_values[i]->set_text(std::to_string(int(mDefense->get_resistance(RESISTANCES(i)))));
	}
	mMove_speed_value->set_text(std::to_string(int(mMove_speed)));
	mHealth_value->set_text(std::to_string(int(mDefense->get_health())));
	
	mClickable_space.x = int(mPosition.x) - mCenter.x;
	mClickable_space.y = int(mPosition.y) - mCenter.y;
	
	update_animation_clip();

	SDL_Rect dest;

	dest.x = static_cast<int>(mPosition.x - mCenter.x);
	dest.y = static_cast<int>(mPosition.y - mCenter.y);
	dest.w = mCurrent_clip.w;
	dest.h = mCurrent_clip.h;
	gLayer_handler->renderex_to_layer(this->mSprite, mRender_layer, &this->mCurrent_clip, &dest, this->get_rotation_angle(), &this->mCenter, SDL_FLIP_NONE);
}

double Unit::get_rotation_angle() const
{
	switch (this->mDirection) {
	case DIRECTION::RIGHT:
		return 0.0;
	case DIRECTION::DOWN:
		return 90.0;
	case DIRECTION::LEFT:
		return 180.0;
	case DIRECTION::UP:
		return 270.0;
	default:
		return 0.0;
	}
}

void Unit::update_animation_clip()
{
	mAnimation_tick++;
	if (mAnimation_tick >= mAnimation_tick_count)
	{
		mAnimation_tick = 0;
	}
	const auto index = mAnimation_tick / mTickcount_per_clip;

	mCurrent_clip = mClips.at(index);
}

void Unit::on_click(int mouse_x, int mouse_y)
{
	SDL_Rect rect;
	rect.x = 1680;
	rect.y = 824;
	rect.w = 200;
	rect.h = 200;
	mLevel->get_menu()->set_unit_window(mUnit_window);
}

Defense* Unit::get_defense()
{
	return mDefense;
}

double Unit::get_move_speed()
{
	return mMove_speed;
}



