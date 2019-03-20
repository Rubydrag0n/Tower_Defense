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
	void set_stat_strings_for_upgrade_buttons(Button* button, std::string* dmg_text, std::string* as_text, std::string* range_text, std::string* dmg_distribution_text) const;

private:
	UpgradeButton* mUpgrade_damage_button;
	UpgradeButton* mUpgrade_range_button;
	UpgradeButton* mUpgrade_attackspeed_button;
	int mNumber_of_damage_upgrades = 0;
	int mNumber_of_attackspeed_upgrades = 0;
	int mNumber_of_range_upgrades = 0;
	LTexture* mDmg_text;
	LTexture* mAs_text;
	LTexture* mRange_text;
	LTexture* mDamage_distribution_headline;
	LTexture* mDamage_distribution_text;
	LTexture* mDamage_upgrade_number_texture;
	LTexture* mAttackspeed_upgrade_number_texture;
	LTexture* mRange_upgrade_number_texture;
	
};
