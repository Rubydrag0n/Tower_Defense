#pragma once
#include "Window.h"
#include "Button.h"
#include "CoordinatesInDouble.h"

class Building;

enum BUILDINGWINDOWBUTTONIDS {
	DEMOLISH_BUTTON,
	UPGRADE_BUTTON,
	UPGRADE_DAMAGE_BUTTON,
	UPGRADE_RANGE_BUTTON,
	UPGRADE_ATTACKSPEED_BUTTON,
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
	
	void demolish_building();
	void upgrade_building();

	Button* get_demolish_button();
	Button* get_upgrade_button();
	CoordinatesInDouble get_button_offset();
	Building* get_building();


private:
	Button* mDemolish_button;
	Button* mUpgrade_button;
	CoordinatesInDouble mButton_offset;

protected:
	Building* mBuilding;
	std::map<Button*, std::string> mUpgrade_buttons;


};
