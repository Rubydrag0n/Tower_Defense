#pragma once
#include <string>
#include <SDL.h>
#include "Ressources.h"
#include <vector>

class Building
{
public:
	Building(std::string building_name, SDL_Point coords);
	~Building();

	void render();
	SDL_Point get_coords() const;
	Ressources get_construction_costs() const;
	Ressources get_maintenance() const;
	void set_coords(SDL_Point coords);
	void set_construction_costs(Ressources costs);
	void set_maintenance(Ressources maintenance);

protected:
	SDL_Point mCoords;
	Ressources mConstruction_costs;
	Ressources mMaintenance;
	SDL_Texture *mSprite;
	SDL_Rect mSprite_dimensions;
	SDL_Rect mClip;
};



