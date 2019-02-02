#pragma once
#include "Clickable.h"
#include "Building.h"


class MenuItem final :
	public Clickable, public Renderable
{
public:
	MenuItem(const std::string& name_of_object, Level *level, SDL_Point coords);
	~MenuItem();

	void render() override;
	void delete_clickable_space();

	void on_click(int mouse_x, int mouse_y) override;


	LTexture* get_sprite() const;
	SDL_Point get_coords() const;
	Resources get_construction_costs() const;


private:
	SDL_Point mCoords; //coordinates, where the item is placed in the menu
	LTexture* mSprite;
	Resources mConstruction_costs;
	std::string mName_of_object; // for example the tower name
	Level *mLevel;
};
