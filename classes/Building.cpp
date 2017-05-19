#include "Building.h"



Building::Building(SDL_Point coords)
{
	set_coords(coords);
}

Building::~Building()
{
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

void Building::set_coords(SDL_Point coords)
{
	mCoords = coords;
}

void Building::set_construction_costs(Ressources cost)
{
	mConstruction_costs = cost;
}

void Building::set_maintenance(Ressources new_maintenance)
{
	mMaintenance = new_maintenance;
}