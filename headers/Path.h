#pragma once
#include "Building.h"
#include "Menu.h"
#include "Enums.h"

class Path final
	: public Building
{
public:
	Path(std::string path_name, SDL_Point coords, Level* level);

	BUILDINGTYPE get_building_type() override;

	double get_speed_multiplier() const;

private:

	//how fast a unit is compared to normal on this road
	double mSpeed_multiplier;
};