#pragma once
#include "Window.h"
#include "Button.h"
#include "CoordinatesInDouble.h"

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
	CoordinatesInDouble get_button_offset();
	void demolish_building();


private:
	Button* mDemolish_button;
	Building* mBuilding;
	CoordinatesInDouble mButton_offset;
};
