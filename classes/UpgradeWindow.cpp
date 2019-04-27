#include "UpgradeWindow.h"
#include "Resources.h"
#include "ConfigFile.h"
#include <iostream>

UpgradeWindow::UpgradeWindow(SDL_Rect dim, std::string building_upgrade_section) : Window(dim, WINDOWS, WINDOWS)
{
	mUpgrade_costs = new Resources();
	mUpgrade_costs->set_resources(gConfig_file->value_or_zero(building_upgrade_section, "goldcosts"),
		gConfig_file->value_or_zero(building_upgrade_section, "woodcosts"),
		gConfig_file->value_or_zero(building_upgrade_section, "stonecosts"),
		gConfig_file->value_or_zero(building_upgrade_section, "ironcosts"),
		gConfig_file->value_or_zero(building_upgrade_section, "energycosts"),
		gConfig_file->value_or_zero(building_upgrade_section, "watercosts"),
		gConfig_file->value_or_zero(building_upgrade_section, "foodcosts"));
	
	mResource_names = new Text*[RESOURCES_TOTAL];
	mUpgrade_cost_values = new Text*[RESOURCES_TOTAL];
	
	SDL_Color text_color = { 0,0,0,0 };
	SDL_Rect dest;
	dest.x = mDim.x + 20;
	dest.y = mDim.y + mDim.h - 180;
	dest.w = 0;	
	dest.h = 0;
	mHeadline = new Text("Upgradecosts", dest, WINDOWBUTTONS, text_color, this);
	for (auto i = 0; i < RESOURCES_TOTAL; ++i)
	{
		dest.y += 20;
		mResource_names[i] = new Text(Resources::get_name(RESOURCETYPES(i)), dest, WINDOWS, text_color, this);
		mUpgrade_cost_values[i] = new Text(std::to_string(mUpgrade_costs->get_resource(RESOURCETYPES(i))), dest, WINDOWBUTTONS, text_color, this);
		mUpgrade_cost_values[i]->add_x_dim(60);
	}
}

UpgradeWindow::~UpgradeWindow()
{

}

void UpgradeWindow::render()
{
	Window::render();
}

