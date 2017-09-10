#pragma once
#include "Level.h"

class Menu
{
public:
	Menu(Level *level);
	~Menu();

	void render();

private:
	Level *mLevel;
};
