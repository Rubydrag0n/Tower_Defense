#pragma once
#include "ShowMoreButton.h"
#include "Text.h"
#include "UpgradeButton.h"

class BigUpgrade
{
public:
	BigUpgrade(std::string obj_name, std::string upgrade_section, UpgradeButton* big_upgrade_button, ShowMoreButton* show_more_button);
	~BigUpgrade();

	ShowMoreButton* get_show_more_button();
	UpgradeButton* get_big_upgrade_button();
	Text* get_upgrade_name();

private:
	UpgradeButton* mBig_upgrade_button;
	ShowMoreButton* mShow_more_button;
	Text* mUpgrade_name;
};
