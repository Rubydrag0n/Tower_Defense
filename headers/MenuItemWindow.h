#pragma once
#include "Window.h"
#include "Resources.h"


class MenuItemWindow :
	public Window
{
public:
	MenuItemWindow(SDL_Rect dim, Resources res);
	~MenuItemWindow();

	void render();

private:
	Resources mConstruction_costs;


};
