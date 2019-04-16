#pragma once
#include "ShowMoreButton.h"
#include "Text.h"
#include "UpgradeButton.h"

class BigUpgrade
{
public:
	BigUpgrade(std::string obj_name, std::string upgrade_section, UpgradeButton* big_upgrade_button, ShowMoreButton* show_more_button);
	~BigUpgrade();

	void shift(int v); //how many pixels it is shifted in y-direction

	ShowMoreButton* get_show_more_button();
	UpgradeButton* get_big_upgrade_button();
	Text* get_upgrade_name();
	bool is_upgrade_description_shown();
	bool is_shifted_down();
	void set_shifted_down(bool v);
	void set_upgrade_description_shown(bool v);

private:
	UpgradeButton* mBig_upgrade_button;
	ShowMoreButton* mShow_more_button;

	Text* mUpgrade_name;
	Text* mUpgrade_description;

	bool mShifted_down; //true if a show-more-button from a other upgrade, that has a smaller y-Value(coord) is clicked, so this upgrade is shifted down, so the description can be shown
};
