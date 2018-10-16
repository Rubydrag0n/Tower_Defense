#include "Path.h"
#include "Building.h"
#include "Enemy.h"
#include "Menu.h"

Path::Path(std::string path_name, SDL_Point coords, Level* level) : Building(path_name, coords, level)
{
	
}

Path::~Path()
{

}

BUILDINGTYPE Path::get_building_type()
{
	return BUILDINGTYPE::STREET;
}

double Path::get_speed_multiplier()
{
	return this->mSpeed_multiplier;
}
