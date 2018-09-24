#include "Button.h"
#include "ConfigFile.h"
#include "SDL_setup.h"
#include "LayerHandler.h"

Button::Button(std::string button_name, SDL_Rect dim, void (*on_click)()) : mClips{}, mButtonSprite()
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
	this->set_on_click_function(on_click);
	this->set_on_mouse_over_function(nullptr);
	this->set_on_right_click_function(nullptr);
	this->set_on_middle_click_function(nullptr);
}

Button::~Button()
{
}

void Button::set_dimension(SDL_Rect dim)
{
	this->mButton_dimensions = dim;
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
	if (mOn_click != nullptr) mOn_click();
}

void Button::on_mouse_over(int mouse_x, int mouse_y)
{
	if (mOn_mouse_over != nullptr) mOn_mouse_over();
}

void Button::on_right_click(int mouse_x, int mouse_y)
{
	if (mOn_right_click != nullptr) mOn_right_click();
}

void Button::on_middle_click(int mouse_x, int mouse_y)
{
	if (mOn_middle_click != nullptr) mOn_middle_click();
}

void Button::set_on_click_function(void(*on_click)())
{
	this->mOn_click = on_click;
}

void Button::set_on_mouse_over_function(void(*on_mouse_over)())
{
	this->mOn_mouse_over = on_mouse_over;
}

void Button::set_on_right_click_function(void(*on_right_click)())
{
	this->mOn_right_click = on_right_click;
}

void Button::set_on_middle_click_function(void(*on_middle_click)())
{
	this->mOn_middle_click = on_middle_click;
}
