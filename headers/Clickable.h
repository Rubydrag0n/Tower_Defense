#pragma once
#include <SDL.h>

class Clickable
{
public:
	Clickable();
	virtual ~Clickable();

	//gets called when the object is clicked
	virtual void on_click() = 0;

	//gets called when the mouse hovers over the object. It gets called every tick this happens
	virtual void on_mouse_over() = 0;
	
	//gets called when the object is clicked with the right mouse button
	virtual void on_right_click() = 0;

	//gets called when the object is clicked with the middle mouse button
	virtual void on_middle_click() = 0;

	SDL_Rect get_clickable_space() const;
	void set_clickable_space(SDL_Rect clickable_space);

private:

	SDL_Rect mClickable_space;
};
