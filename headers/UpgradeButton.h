#pragma once
#include "Button.h"

class UpgradeButton : public Button
{
public:
	UpgradeButton(const std::string& button_name, SDL_Rect dim, ButtonObject* obj, std::string upgrade_section, int button_id = 0);
	~UpgradeButton();

	std::string get_upgrade_section();

	void render() override;

private:
	std::string mUpgrade_section;

};
