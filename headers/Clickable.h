#pragma once
#include <SDL.h>

class Clickable
{
public:
	virtual ~Clickable()
	{
	}

	//Handles mouse event
	virtual void handleEvent(SDL_Event* e) = 0;
};