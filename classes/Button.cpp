#include "Button.h"
#include "ConfigFile.h"
#include "SDL_setup.h"
#include "LayerHandler.h"
#include "ButtonObject.h"
#include "BuildingWindow.h"

Button::Button(const std::string& button_name, const SDL_Rect dim, ButtonObject* obj, Renderable* texture_to_render_on, LAYERS click_layer, LAYERS render_layer, const int button_id) : Clickable(click_layer), Renderable(render_layer), mButton_dimensions(dim), mClips{}, mButton_sprite{}, mObject_to_notify(obj), mTexture_to_render_on(texture_to_render_on), mButton_id{ button_id }
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

void Button::add_x_dimension(int x)
{
	mButton_dimensions.x += x;
}

void Button::add_y_dimension(int y)
{
	mButton_dimensions.y += y;
}

SDL_Rect Button::get_dimension() const
{
	return mButton_dimensions;
}

Renderable* Button::get_texture_to_render_on()
{
	return mTexture_to_render_on;
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
	if(mTexture_to_render_on != nullptr)
	{
		if(mTexture_to_render_on->is_rendering_enabled())
		{
			gLayer_handler->render_to_layer(mButton_sprite, mRender_layer, &mClips[this->get_state()], &mButton_dimensions);
		}
	}
	else
	{
		gLayer_handler->render_to_layer(mButton_sprite, mRender_layer, &mClips[this->get_state()], &mButton_dimensions);
	}
}

void Button::on_click(int mouse_x, int mouse_y)
{
	this->mObject_to_notify->on_button_press(this->mButton_id, this);
}
