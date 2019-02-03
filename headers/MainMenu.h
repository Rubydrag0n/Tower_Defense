#pragma once
#include "LTexture.h"

//TODO: Unused class?!
class MainMenu
{
public:
	MainMenu();

	void render() const;
private:
	LTexture *mSprite;
	std::string mSprite_path;
};
