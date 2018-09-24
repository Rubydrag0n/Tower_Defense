#include "CoordinatesInDouble.h"
#include <SDL.h>

CoordinatesInDouble::CoordinatesInDouble()
{
	x = 0;
	y = 0;
}

CoordinatesInDouble::CoordinatesInDouble(SDL_Point p)
{
	this->x = p.x;
	this->y = p.y;
}

CoordinatesInDouble::~CoordinatesInDouble()
{
	
}

CoordinatesInDouble CoordinatesInDouble::operator+(CoordinatesInDouble summand) const
{
	CoordinatesInDouble result;
	result.x = this->x + summand.x;
	result.y = this->y + summand.y;
	return result;
}

CoordinatesInDouble::operator SDL_Point()
{
	SDL_Point p;
	p.x = int(this->x);
	p.y = int(this->y);
	return p;
}
