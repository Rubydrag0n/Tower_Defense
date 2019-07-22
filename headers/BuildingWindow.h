#pragma once
#include "Window.h"
#include "Button.h"
#include "CoordinatesInDouble.h"
#include <vector>
#include "Text.h"


class Building;
class BigUpgrade;

class BuildingWindow
	: public Window, public ButtonObject
{
public:
	BuildingWindow(SDL_Rect dim, Building* building);
	~BuildingWindow();

	void render() override;
	void on_button_press(int button_id, Button* button) override;
	
	void upgrade_building(Button* button);//button must be from class UpgradeButton
	void show_more(Button* button); //
	void update_great_upgrades();

	std::vector<BigUpgrade*> get_big_upgrades();

	CoordinatesInDouble get_button_offset() const;
	Building* get_building() const;

private:
	CoordinatesInDouble mButton_offset;

	//the textures can't be immediately deleted, so they are reused instead
	
	Text** mStorage_values;
	Text** mMaintenance_values;
	Text* mHeadline;

protected:
	Text** mResource_names;
	Building* mBuilding;
	SDL_Color mText_color{};
	std::vector<BigUpgrade*> mBig_upgrades; 
};
