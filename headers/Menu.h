#pragma once
#include "Level.h"
#include "MenuItem.h"


class Menu
{
public:
	Menu(Level *level);
	~Menu();

	void render(); //shows lives and ressources

private:
	Level* mLevel;
	LTexture* mMenu_texture;
	std::vector<MenuItem*> mMenu_items;
};
