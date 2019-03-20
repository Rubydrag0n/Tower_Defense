#include "BigUpgradeButton.h"
#include "LayerHandler.h"
#include "ConfigFile.h"

BigUpgradeButton::BigUpgradeButton(std::string obj_to_upgrade_name, const std::string& button_name, SDL_Rect dim, ButtonObject* obj, std::string upgrade_section, int button_id) : UpgradeButton(button_name, dim, obj, upgrade_section, button_id)
{
	SDL_Color text_color = { 0,0,0,0 };
	std::string upgrade_name;
	upgrade_name.assign(gConfig_file->value(obj_to_upgrade_name + "/upgrade" + upgrade_section, "name"));
	mUpgrade_name->load_from_rendered_text(upgrade_name, text_color);
}

BigUpgradeButton::~BigUpgradeButton()
{
	
}

void BigUpgradeButton::render()
{
	UpgradeButton::render();
	auto dest = mButton_dimensions;
	dest.x -= 130; //upgradename is left of the upgrade button
	dest.w = 0;
	dest.h = 0;
	gLayer_handler->render_to_layer(mUpgrade_name, WINDOWS, nullptr, &dest);
}


