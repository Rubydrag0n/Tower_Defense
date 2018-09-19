#pragma once
#include <vector>
#include "Clickable.h"
#include <SDL.h>

class LTexture;

class MouseHandler
{
public:
	MouseHandler();
	~MouseHandler();

	void add_clickable(Clickable* c);
	void del_clickable(Clickable* c);

	//handles the mouse_over events
	void update();
	
	//handles all click related events
	void handle_event(SDL_Event* e);

	//renders what the mouse holds
	void render(SDL_Rect dest, SDL_Point mouse_position, LTexture* sprite);

	Clickable* get_item_on_mouse();
	void set_item_on_mouse(Clickable* item);

private:
	std::vector<Clickable*> mClickables;
<<<<<<< HEAD
	Clickable* mItem_on_mouse;
=======


>>>>>>> f6e2745cf2f6de1479f8fb1bbecd57b07ec75a50
};

//holds pointers to all clickable objects
extern MouseHandler* gMouse_handler;
