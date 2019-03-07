#pragma once
#include "Window.h"
#include "Button.h"
#include "CoordinatesInDouble.h"
#include <vector>

class Building;

class BuildingWindow
	: public Window, public ButtonObject
{
public:
	BuildingWindow(SDL_Rect dim, Building* building);
	~BuildingWindow();

	void render() override;
	void on_button_press(int button_id, Button* button) override;
	
	void demolish_building() const;
	void upgrade_building(Button* button);//button must be from class UpgradeButton
	void update_great_upgrade_buttons();

	Button* get_demolish_button() const;
	std::vector<Button*> get_upgrade_buttons();
	CoordinatesInDouble get_button_offset() const;
	Building* get_building() const;


private:
	Button* mDemolish_button;
	CoordinatesInDouble mButton_offset;

	//the textures can't be immediately deleted, so they are reused instead
	LTexture** mText;
	LTexture* mHeadline;

	SDL_Color mText_color;

protected:
	Building* mBuilding;

	//map with the buttons that can be used to make great upgrades on the current building level; the string marks the upgrade(for example: 1.3.4.1)
	std::vector<Button*> mUpgrade_buttons; 
};
