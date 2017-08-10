#include "Building.h"
#include "ConfigFile.h"
#include "SDL_setup.h"


Building::Building(std::string building_name, SDL_Point coords)
{
	ConfigFile cf("config/game.cfg");
	mSprite = loadTexture(cf.Value(building_name + "/sprite", "path"));
	mSprite_dimensions.w = cf.Value(building_name + "/sprite", "image_width");
	mSprite_dimensions.h = cf.Value(building_name + "/sprite", "image_height");

	set_coords(coords);
}

Building::~Building()
{
	SDL_DestroyTexture(mSprite);
}

void Building::render()
{
	
}
void Building::set_construction_costs(Ressources costs)
{
	mConstruction_costs = costs;
}

void Building::set_maintenance(Ressources new_maintenance)
{
	mMaintenance = new_maintenance;
}

void Building::set_coords(SDL_Point coords)
{
	mCoords = coords;
}


SDL_Point Building::get_coords() const
{
	return mCoords;
}

Ressources Building::get_construction_costs() const
{
	return mConstruction_costs;
}

Ressources Building::get_maintenance() const
{
	return mMaintenance;
}


