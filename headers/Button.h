#pragma once
#include <SDL.h>
#include "LTexture.h"
#include "Clickable.h"
#include "Renderable.h"
#include "ButtonObject.h"

class Button final
	: public Clickable, public Renderable
{
public:
	//creates a button with dimensions (and position) dim and a function onclick that gets called when the button is clicked on
	Button(const std::string& button_name, SDL_Rect dim, ButtonObject* obj, int button_id = 0);
	~Button();

	//Sets dimension of the button on the screen
	void set_dimension(SDL_Rect dim);
	SDL_Rect get_dimension() const;

	//Sets the Clips
	void set_sprite_clips(SDL_Rect *clips);

	//Shows button sprite
	void render() override;

	void on_click(int mouse_x, int mouse_y) override;

private:
	//Dimensions of the Button
	SDL_Rect mButton_dimensions;

	//The Position of the Clips
	SDL_Rect mClips[STATES_TOTAL];

	//Texture of the Button
	LTexture* mButton_sprite;

	//Object that wants to be notified of click on the button
	ButtonObject* mObject_to_notify;

	//button id is transferred in case more than one button can call the function on object_to_notify
	int mButton_id;

	std::string mButton_name;
};
