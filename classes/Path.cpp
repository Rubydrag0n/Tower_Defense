#include "Path.h"
#include "Building.h"
#include "Menu.h"

Path::Path(std::string path_name, SDL_Point coords, Level* level) : Building(std::move(path_name), coords, level), mSpeed_multiplier()
{
	SDL_Rect rect;
	rect.x = mCoords.x;
	rect.y = mCoords.y - 200;
	rect.w = 200;
	rect.h = 200;
	mWindow = new BuildingWindow(rect, this);
}

BUILDINGTYPE Path::get_building_type()
{
	return BUILDINGTYPE::STREET;
}

double Path::get_speed_multiplier() const
{
	return this->mSpeed_multiplier;
}
