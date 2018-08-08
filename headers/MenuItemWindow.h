#pragma once
#include "Window.h"
#include "Resources.h"


class MenuItemWindow :
	public Window
{
public:
	MenuItemWindow(SDL_Rect dim, Ressources res);
	~MenuItemWindow();

	void render();

private:
	Ressources mConstruction_costs;


};
