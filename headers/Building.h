#pragma once
#include <SDL.h>
#include "Resources.h"
#include <string>
#include "Clickable.h"
#include "LTexture.h"

class Level;

class Building : public Clickable
{
public:
	Building(std::string building_name, SDL_Point coords, Level *level);
	~Building();

	virtual void render() const;
	void place();
	virtual void update();

	SDL_Point get_coords() const;
	Ressources get_construction_costs() const;
	Ressources get_maintenance() const;
	void set_coords(SDL_Point coords);
	void set_maintenance(Ressources maintenance);

	void on_click(int mouse_x, int mouse_y) override;
	void on_mouse_over(int mouse_x, int mouse_y) override;
	void on_right_click(int mouse_x, int mouse_y) override;
	void on_middle_click(int mouse_x, int mouse_y) override;

protected:
	SDL_Point mCoords;
	Ressources mConstruction_costs;
	Ressources mMaintenance;
	LTexture *mSprite;
	SDL_Rect mSprite_dimensions;
	Level *mLevel;

	std::string mName;
	std::string mSprite_path;
};



