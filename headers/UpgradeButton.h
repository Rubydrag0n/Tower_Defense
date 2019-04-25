#pragma once
#include "Button.h"
#include "WindowButton.h"

class UpgradeButton : public WindowButton
{
public:
	UpgradeButton(const std::string& button_name, SDL_Rect dim, ButtonObject* obj, std::string upgrade_section, LAYERS click_layer, LAYERS render_layers, Window* window, int button_id = 0);
	~UpgradeButton();

	std::string get_upgrade_section();

private:
	std::string mUpgrade_section;

};
