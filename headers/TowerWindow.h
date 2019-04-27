#pragma once
#include "BuildingWindow.h"
#include "UpgradeButton.h"

class Tower;

class TowerWindow : public BuildingWindow
{
public:
	TowerWindow(SDL_Rect dim, Tower* tower);
	~TowerWindow();

	void render() override;
	void on_button_press(int button_id, Button* button) override;

	void upgrade_damage();
	void upgrade_range();
	void upgrade_attackspeed();

	Button* get_upgrade_damage_button() const;
	Button* get_upgrade_range_button() const;
	Button* get_upgrade_attackspeed_button() const;

	//if an upgradebutton is hovered this function is called and sets the strings with the stats for the upgrade, that are shown in the window
	void set_stat_strings_for_upgrade_buttons(Button* button);

private:
	UpgradeButton* mUpgrade_damage_button;
	UpgradeButton* mUpgrade_range_button;
	UpgradeButton* mUpgrade_attackspeed_button;
	int mNumber_of_damage_upgrades = 0;
	int mNumber_of_attackspeed_upgrades = 0;
	int mNumber_of_range_upgrades = 0;
	Text* mDmg_text;
	Text* mDmg_value;
	Text* mAs_text;
	Text* mAs_value;
	Text* mRange_text;
	Text* mRange_value;
	Text* mDamage_distribution_headline;
	Text* mDamage_distribution_text;
	Text* mDamage_upgrade_number_texture;
	Text* mAttackspeed_upgrade_number_texture;
	Text* mRange_upgrade_number_texture;
	
};
