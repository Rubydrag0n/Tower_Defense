#pragma once
#include "Building.h"
#include "Menu.h"
#include "Enums.h"

class Path
	: public Building
{
public:
	Path(std::string path_name, SDL_Point coords, Level* level);
	~Path();

	BUILDINGTYPE get_building_type() override;

	double get_speed_multiplier();

private:

	//how fast a unit is compared to normal on this road
	double mSpeed_multiplier;
};