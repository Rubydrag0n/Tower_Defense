#pragma once
#include <vector>
#include "Ressources.h"
#include "SDL.h"

class Building
{
public:
	Building(SDL_Point coords);
	~Building();
	
	virtual void update();

	SDL_Point get_coords() const;
	Ressources get_construction_costs() const;
	Ressources get_maintenance() const;
	void set_coords(SDL_Point coords);
	void set_construction_costs(Ressources costs);
	void set_maintenance(Ressources maintenance);


private:
	SDL_Point mCoords;
	Ressources mConstruction_costs;
	Ressources mMaintenance;

};
