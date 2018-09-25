#pragma once
#include "Clickable.h"
#include "Building.h"
#include "MenuItemWindow.h"


class MenuItem :
	public Clickable, public Renderable
{
public:
	MenuItem(std::string name_of_object, Level *level, SDL_Point coords);
	~MenuItem();

	void render();
	void delete_clickable_space();

	void on_click(int mouse_x, int mouse_y) override;
	void on_mouse_over(int mouse_x, int mouse_y) override;
	void on_right_click(int mouse_x, int mouse_y) override;
	void on_middle_click(int mouse_x, int mouse_y) override;


	LTexture* get_sprite();
	SDL_Point get_coords();
	Resources get_construction_costs() const;


private:
	SDL_Point mCoords; //coordinates, where the item is placed in the menu
	LTexture* mSprite;
	Resources mConstruction_costs;
	std::string mName_of_object; // for example the tower name
	Level *mLevel;
	MenuItemWindow* mWindow; //the window that is shown on mouse over
};
