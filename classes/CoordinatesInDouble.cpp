#include "CoordinatesInDouble.h"

CoordinatesInDouble::CoordinatesInDouble()
{
	x = 0;
	y = 0;
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


