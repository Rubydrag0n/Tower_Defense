#pragma once
#include <SDL.h>
#include "Ressources.h"
#include <string>

class Level;

class Building
{
public:
	Building(std::string building_name, SDL_Point coords, Level *level);
	~Building();

	void render();
	void select();
	void place();
	void update();

	SDL_Point get_coords() const;
	Ressources get_construction_costs() const;
	Ressources get_maintenance() const;
	void set_coords(SDL_Point coords);
	void set_maintenance(Ressources maintenance);
	

protected:
	SDL_Point mCoords;
	Ressources mConstruction_costs;
	Ressources mMaintenance;
	SDL_Texture *mSprite;
	SDL_Rect mSprite_dimensions;
	Level *mLevel;
};



