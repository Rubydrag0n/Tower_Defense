#pragma once
#include <SDL.h>

enum LClickableState
{
	MOUSE_OUT = 0,			//while the mouse is not over the clickable
	MOUSE_OVER = 1,			//while the mouse hovers over the clickable
	MOUSE_DOWN_LEFT = 2,	//while left mouse button is pressed
	MOUSE_DOWN_RIGHT = 3,	//while right mouse button is pressed
	MOUSE_DOWN_MIDDLE = 4,	//while middle mouse button is pressed
	MOUSE_UP = 5,			//one tick after a mouseclick
	STATES_TOTAL = 6
};

class Clickable
{
public:
	Clickable();
	virtual ~Clickable();

	//gets called when the object is clicked
	virtual void on_click(int mouse_x, int mouse_y) = 0;

	//gets called when the mouse hovers over the object. It gets called every tick this happens
	virtual void on_mouse_over(int mouse_x, int mouse_y) = 0;
	
	//gets called when the object is clicked with the right mouse button
	virtual void on_right_click(int mouse_x, int mouse_y) = 0;

	//gets called when the object is clicked with the middle mouse button
	virtual void on_middle_click(int mouse_x, int mouse_y) = 0;

	SDL_Rect get_clickable_space() const;
	void set_clickable_space(SDL_Rect clickable_space);

	LClickableState get_state() const;
	void set_state(LClickableState state);

private:
	//the space that can be clicked on to trigger an event
	SDL_Rect mClickable_space;

	//the state that the clickable is in right now. Is handled by the MouseHandler
	LClickableState mState;
};
