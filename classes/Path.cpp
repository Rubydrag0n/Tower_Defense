#include "Path.h"
#include "Building.h"
#include "Menu.h"
#include "ConfigFile.h"

Path::Path(const std::string& path_name, const SDL_Point coords, Level* level) : Building(path_name, coords, level), mSpeed_multiplier()
{
	SDL_Rect rect;
	rect.x = mCoords.x;
	rect.y = mCoords.y - 200;
	rect.w = 200;
	rect.h = 200;
	mWindow = new BuildingWindow(rect, this);

	mSpeed_multiplier = gConfig_file->value(path_name + "/stats", "speed_multiplier");
}

BUILDINGTYPE Path::get_building_type()
{
	return STREET;
}

double Path::get_speed_multiplier() const
{
	return this->mSpeed_multiplier;
}
