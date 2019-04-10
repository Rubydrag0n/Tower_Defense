#pragma once
#include "Window.h"
#include "Button.h"
#include "CoordinatesInDouble.h"
#include <vector>
#include "UpgradeButton.h"

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
	void show_more(Button* button); //
	void update_great_upgrade_buttons();

	Button* get_demolish_button() const;
	std::vector<UpgradeButton*> get_upgrade_buttons();
	std::vector<Button*> get_show_more_buttons();
	CoordinatesInDouble get_button_offset() const;
	Building* get_building() const;


private:
	Button* mDemolish_button;
	CoordinatesInDouble mButton_offset;

	//the textures can't be immediately deleted, so they are reused instead
	LTexture** mText;
	LTexture* mHeadline;

	

protected:
	Building* mBuilding;
	SDL_Color mText_color;
	std::vector<UpgradeButton*> mUpgrade_buttons; 
	std::vector<Button*> mShow_more_buttons;
};
