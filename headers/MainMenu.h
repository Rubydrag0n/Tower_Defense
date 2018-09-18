#pragma once
#include "LTexture.h"

class MainMenu
{
public:
	MainMenu();
	~MainMenu();

	void render();
private:
	LTexture *mSprite;
	std::string mSprite_path;
};
