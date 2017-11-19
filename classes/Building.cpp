#include "Building.h"
#include "ConfigFile.h"
#include "SDL_setup.h"
#include <SDL.h>
#include "Level.h"


Building::Building(std::string building_name, SDL_Point coords, Level* level)
{
	ConfigFile cf("config/game.cfg");

	//load texture and the size of the image from the config file
	mSprite = gTextures->get_texture(cf.Value(building_name + "/sprite", "path"));
	mSprite_dimensions.w = cf.Value(building_name + "/sprite", "image_width");
	mSprite_dimensions.h = cf.Value(building_name + "/sprite", "image_height");
	mSprite_dimensions.x = 0;
	mSprite_dimensions.y = 0;

	//set the construction costs of the building
	mConstruction_costs.set_ressources(cf.Value(building_name + "/stats", "goldcosts"),
									   cf.Value(building_name + "/stats", "woodcosts"),
									   cf.Value(building_name + "/stats", "stonecosts"),
									   cf.Value(building_name + "/stats", "ironcosts"),
									   cf.Value(building_name + "/stats", "energycosts"),
									   cf.Value(building_name + "/stats", "watercosts"),
									   cf.Value(building_name + "/stats", "foodcosts"));

	//set the maintenance costs of the building
	mMaintenance.set_ressources(cf.Value(building_name + "/stats", "goldMain"),
		cf.Value(building_name + "/stats", "woodMain"),
		cf.Value(building_name + "/stats", "stoneMain"),
		cf.Value(building_name + "/stats", "ironMain"),
		cf.Value(building_name + "/stats", "energyMain"),
		cf.Value(building_name + "/stats", "waterMain"),
		cf.Value(building_name + "/stats", "foodMain"));

	mLevel = level;
	mCoords = coords;
}

Building::~Building()
{
	SDL_DestroyTexture(mSprite);
}

//render the picture of the building
void Building::render() const
{
	SDL_Rect dest;
	dest.x = mCoords.x - mSprite_dimensions.w / 2;
	dest.y = mCoords.y - mSprite_dimensions.h / 2;
	dest.w = mSprite_dimensions.w;
	dest.h = mSprite_dimensions.h;

	SDL_RenderCopy(gRenderer, mSprite, &mSprite_dimensions, &dest);
}

void Building::update()
{
	mLevel->get_ressources()->sub(&mMaintenance);
}

void Building::select()
{
	
}

void Building::place()
{
	mLevel->get_ressources()->sub(&mConstruction_costs);
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


