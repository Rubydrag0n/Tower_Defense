#include "UpgradeButton.h"

UpgradeButton::UpgradeButton(const std::string& button_name, SDL_Rect dim, ButtonObject* obj, std::string upgrade_section, int button_id) : Button(button_name, dim, obj, button_id), mUpgrade_section(upgrade_section)
{
	
}

UpgradeButton::~UpgradeButton()
{
	
}

std::string UpgradeButton::get_upgrade_section()
{
	return mUpgrade_section;
}
