#pragma once
#include <SDL.h>
#include "LTexture.h"
#include "Clickable.h"
#include "Renderable.h"

class Button 
	: public Clickable
	, public Renderable
{
public:
	//creates a button with dimensions (and position) dim and a function onclick that gets called when the button is clicked on
	Button(std::string button_name, SDL_Rect dim, void(*on_click)());
	~Button();

	//Sets dimension of the button on the screen
	void set_dimension(SDL_Rect dim);

	//Sets the Clips
	void set_sprite_clips(SDL_Rect *clips);

	//Shows button sprite
	void render() override;

	void on_click(int mouse_x, int mouse_y) override;
	void on_mouse_over(int mouse_x, int mouse_y) override;
	void on_right_click(int mouse_x, int mouse_y) override;
	void on_middle_click(int mouse_x, int mouse_y) override;

	void set_on_click_function(void(*on_click)());
	void set_on_mouse_over_function(void(*on_mouse_over)());
	void set_on_right_click_function(void(*on_right_click)());
	void set_on_middle_click_function(void(*on_middle_click)());

private:
	//Dimensions of the Button
	SDL_Rect mButton_dimensions;

	//The Position of the Clips
	SDL_Rect mClips[STATES_TOTAL];

	//Texture of the Button
	LTexture* mButtonSprite;

	void(*mOn_click)();
	void(*mOn_mouse_over)();
	void(*mOn_right_click)();
	void(*mOn_middle_click)();
};
