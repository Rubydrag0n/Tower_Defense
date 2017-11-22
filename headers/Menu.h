#pragma once
#include "Level.h"

class Menu
{
public:
	Menu(Level *level);
	~Menu();

	void render() const; //shows lives and ressources

private:
	Level *mLevel;
};
