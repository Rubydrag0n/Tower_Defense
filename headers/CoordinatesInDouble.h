#pragma once
#include <SDL.h>

class CoordinatesInDouble
{
public:
	CoordinatesInDouble();
	~CoordinatesInDouble();
	
	double x;
	double y;

	CoordinatesInDouble operator+(CoordinatesInDouble) const;
	operator SDL_Point();
};