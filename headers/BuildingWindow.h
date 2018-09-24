#pragma once
#include "Window.h"
#include "Button.h"

class Building;

class BuildingWindow: public Window
{
public:
	BuildingWindow(SDL_Rect dim, Building* building);
	~BuildingWindow();

	void render() override;
	void delete_button();
private:
	Button* mDemolish_button;
};
