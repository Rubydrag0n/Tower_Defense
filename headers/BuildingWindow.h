#pragma once
#include "Window.h"
#include "Button.h"

class Building;

enum BUILDINGWINDOWBUTTONIDS {
	DEMOLISH_BUTTON,
	BUTTON_NUMBER
};

class BuildingWindow
	: public Window, public ButtonObject
{
public:
	BuildingWindow(SDL_Rect dim, Building* building);
	~BuildingWindow();

	void render() override;
	void on_button_press(int button_id) override;
	
	Button* get_demolish_button();
	void demolish_building();


private:
	Button* mDemolish_button;
	Building* mBuilding;
};
