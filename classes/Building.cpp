#include "Building.h"
#include "ConfigFile.h"
#include "SDL_setup.h"


Building::Building(std::string building_name)
{
	ConfigFile cf("config/game.cfg");
	mSprite = loadTexture(cf.Value(building_name + "/sprite", "path"));
	mSprite_dimensions.w = cf.Value(building_name + "/sprite", "image_width");
	mSprite_dimensions.h = cf.Value(building_name + "/sprite", "image_height");
	 

	mPosition.x = 0;
	mPosition.y = 0;

Building::Building()
{
}

void Building::set_construction_costs(Ressources cost)
{
	mConstruction_costs = cost;
}

void Building::set_maintenance(Ressources new_maintenance)
{
}
