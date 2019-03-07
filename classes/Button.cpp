#include "Button.h"
#include "ConfigFile.h"
#include "SDL_setup.h"
#include "LayerHandler.h"
#include "ButtonObject.h"
#include "BuildingWindow.h"

Button::Button(const std::string& button_name, const SDL_Rect dim, ButtonObject* obj, const int button_id) : mButton_dimensions(dim), mClips{}, mButton_sprite{}, mObject_to_notify(obj), mButton_id{ button_id }
{
	const auto section = "button/" + button_name;
	
	this->mButton_sprite = gTextures->get_texture(gConfig_file->value(section, "path"));

	//initialize the clips
	const int clip_width = gConfig_file->value(section, "clip_width");
	const int clip_height = gConfig_file->value(section, "clip_height");
	for (auto i = 0; i < L_CLICKABLE_STATE::STATES_TOTAL; i++)
	{
		this->mClips[i].x = i*clip_width;
		this->mClips[i].y = 0;
		this->mClips[i].w = clip_width;
		this->mClips[i].h = clip_height;	
	}
	this->set_clickable_space(dim);
}

Button::~Button()
{
	
}

void Button::set_dimension(const SDL_Rect dim)
{
	this->mButton_dimensions = dim;
}

SDL_Rect Button::get_dimension() const
{
	return mButton_dimensions;
}


void Button::set_sprite_clips(SDL_Rect * clips)
{
	for (auto i = 0; i < L_CLICKABLE_STATE::STATES_TOTAL; i++)
	{
		mClips[i] = clips[i];
	}
}

void Button::render()
{
	//Show current button sprite
	gLayer_handler->render_to_layer(mButton_sprite, LAYERS::OVERLAY, &mClips[this->get_state()], &mButton_dimensions);
}

void Button::on_click(int mouse_x, int mouse_y)
{
	this->mObject_to_notify->on_button_press(this->mButton_id, this);
}
