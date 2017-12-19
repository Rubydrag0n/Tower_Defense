#pragma once
#include "Level.h"
#include "MenuItem.h"
#include "CoordinatesInDouble.h"


class Menu
{
public:
	Menu(Level *level);
	~Menu();

	void render(SDL_Point mouse_position); //shows lives and ressources
	void add_menu_item(MenuItem* menu_item);

private:
	Level* mLevel;
	LTexture* mMenu_texture;
	std::vector<MenuItem*> mMenu_items;
};
