#pragma once
#include "Level.h"

class Menu
{
public:
	Menu(Level *level);
	~Menu();

	void render(); //shows lives and ressources

private:
	Level* mLevel;
	LTexture* mMenu_texture;
};
