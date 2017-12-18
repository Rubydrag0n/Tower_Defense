#pragma once

class CoordinatesInDouble
{
public:
	CoordinatesInDouble();
	~CoordinatesInDouble();
	
	double x;
	double y;

	CoordinatesInDouble operator+(CoordinatesInDouble) const;
};