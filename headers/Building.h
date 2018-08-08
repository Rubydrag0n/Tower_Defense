#pragma once
#include <SDL.h>
#include "Resources.h"
#include <string>
#include "Clickable.h"
#include "LTexture.h"
#include "Entity.h"
#include "Window.h"

class Level;

class Building : public Clickable, public Entity
{
public:
	Building(std::string building_name, SDL_Point coords, Level *level);
	~Building();

	void render() override; //render the picture of the building
	void on_tick() override; //takes ressources for maintenance

	SDL_Point get_coords() const;
	Resources get_maintenance() const;
	void set_coords(SDL_Point coords);
	void set_maintenance(Resources maintenance);

	void on_click(int mouse_x, int mouse_y) override;
	void on_mouse_over(int mouse_x, int mouse_y) override;
	void on_right_click(int mouse_x, int mouse_y) override;
	void on_middle_click(int mouse_x, int mouse_y) override;

protected:
	SDL_Point mCoords;
	Resources mMaintenance;
	LTexture *mSprite; //texture
	SDL_Rect mSprite_dimensions; //size of the texture
	Level *mLevel;

	bool idle; // if the player doesnt have enough res to maintain the building the building doesnt do anything
	int mElapsed_ticks;
	std::string mName;
	std::string mSprite_path;

	Window *mWindow;	//the window in which the stats and stuff of the tower can be displayed
};



