#include "BigUpgrade.h"
#include "ConfigFile.h"

BigUpgrade::BigUpgrade(std::string obj_name, std::string upgrade_section, UpgradeButton* big_upgrade_button, ShowMoreButton* show_more_button) : mBig_upgrade_button(big_upgrade_button), mShow_more_button(show_more_button)
{
	SDL_Color text_color = { 0,0,0,0 };
	auto dim_of_upgrade_name_texture = big_upgrade_button->get_dimension();
	const auto y_difference = 130; // y difference between bigupgradebutton and the shown upgradename
	dim_of_upgrade_name_texture.x -= y_difference;
	dim_of_upgrade_name_texture.h = 0;
	dim_of_upgrade_name_texture.h = 0;
	std::string upgrade_name;
	upgrade_name.assign(gConfig_file->value(obj_name + "/upgrade" + upgrade_section, "name"));
	mUpgrade_name = new Text(upgrade_name, dim_of_upgrade_name_texture, WINDOWBUTTONS, text_color, mBig_upgrade_button->get_texture_to_render_on());
}

BigUpgrade::~BigUpgrade()
{
	
}

UpgradeButton* BigUpgrade::get_big_upgrade_button()
{
	return mBig_upgrade_button;
}

ShowMoreButton* BigUpgrade::get_show_more_button()
{
	return mShow_more_button;
}

Text* BigUpgrade::get_upgrade_name()
{
	return mUpgrade_name;
}


 