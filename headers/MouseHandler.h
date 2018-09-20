#pragma once
#include <vector>
#include "Clickable.h"
#include <SDL.h>
#include "MouseItem.h"

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

	MouseItem* get_item_on_mouse();
	void set_item_on_mouse(MouseItem* item);

private:
	std::vector<Clickable*> mClickables;

	MouseItem* mItem_on_mouse;




};

//holds pointers to all clickable objects
extern MouseHandler* gMouse_handler;
