#pragma once
#include <SDL.h>

class CoordinatesInDouble
{
public:
	CoordinatesInDouble();
	explicit CoordinatesInDouble(SDL_Point p);
	
	double x;
	double y;

	CoordinatesInDouble operator+(CoordinatesInDouble) const;
	operator SDL_Point() const;
};