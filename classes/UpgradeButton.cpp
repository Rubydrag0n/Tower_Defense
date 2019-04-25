#include "UpgradeButton.h"
#include "ConfigFile.h"
#include "LayerHandler.h"

UpgradeButton::UpgradeButton(const std::string& button_name, SDL_Rect dim, ButtonObject* obj, std::string upgrade_section, LAYERS click_layer, LAYERS render_layers, Window* window, int button_id) : WindowButton(button_name, dim, obj, click_layer, render_layers, window, button_id), mUpgrade_section(upgrade_section)
{

}

UpgradeButton::~UpgradeButton()
{
	
}



std::string UpgradeButton::get_upgrade_section()
{
	return mUpgrade_section;
}




