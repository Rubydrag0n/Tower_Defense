#pragma once
#include <vector>
#include "Clickable.h"

class MouseHandler
{
public:
	MouseHandler();
	~MouseHandler();

	void add_clickable(Clickable* c);
	void del_clickable(Clickable* c);

	void handle_event(SDL_Event* e);

private:
	std::vector<Clickable*> mClickables;
};

//holds pointers to all clickable objects
extern MouseHandler* gMouse_handler;
