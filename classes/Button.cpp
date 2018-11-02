#include "Button.h"
#include "ConfigFile.h"
#include "SDL_setup.h"
#include "LayerHandler.h"
#include "ButtonObject.h"
#include "BuildingWindow.h"

Button::Button(std::string button_name, SDL_Rect dim, ButtonObject* obj, int button_id) : mClips{}, mButtonSprite{}, mButton_id{ button_id }
{
	auto section = "button/" + button_name;

	this->mButtonSprite = gTextures->get_texture(gConfig_file->Value(section, "path"));

	//initialize the clips
	int clip_width = gConfig_file->Value(section, "clip_width");
	int clip_height = gConfig_file->Value(section, "clip_height");
	for (auto i = 0; i < LClickableState::STATES_TOTAL; i++)
	{
		this->mClips[i].x = i*clip_width;
		this->mClips[i].y = 0;
		this->mClips[i].w = clip_width;
		this->mClips[i].h = clip_height;	
	}

	this->set_clickable_space(dim);
	this->set_dimension(dim);

	this->mObject_to_notify = obj;
}

Button::~Button()
{
}

void Button::set_dimension(SDL_Rect dim)
{
	this->mButton_dimensions = dim;
}

SDL_Rect Button::get_dimension()
{
	return mButton_dimensions;
}


void Button::set_sprite_clips(SDL_Rect * clips)
{
	for (auto i = 0; i < LClickableState::STATES_TOTAL; i++)
	{
		mClips[i] = clips[i];
	}
}

void Button::render()
{
	//Show current button sprite
	gLayer_handler->render_to_layer(mButtonSprite, LAYERS::OVERLAY, &mClips[this->get_state()], &mButton_dimensions);
}

void Button::on_click(int mouse_x, int mouse_y)
{
	this->mObject_to_notify->on_button_press(this->mButton_id);
}
