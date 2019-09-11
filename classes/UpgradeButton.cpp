#include "UpgradeButton.h"
#include "ConfigFile.h"
#include "LayerHandler.h"
#include "Building.h"

UpgradeButton::UpgradeButton(const std::string& button_name, SDL_Rect dim, ButtonObject* obj, std::string building_name, std::string upgrade_section, LAYERS click_layer, LAYERS render_layers, Window* window, int button_id) : WindowButton(button_name, dim, obj, click_layer, render_layers, window, button_id), mUpgrade_section(upgrade_section)
{
	SDL_Rect dest;
	dest.x = 1480;
	dest.y = 824;
	dest.w = 200;
	dest.h = 200;
	std::string building_upgrade_section;
	if (upgrade_section == "Damage" || upgrade_section == "Attackspeed" || upgrade_section == "Range")
	{
		building_upgrade_section = "Tower/upgrade" + upgrade_section;
	}
	else
	{
		building_upgrade_section = building_name + "/upgrade" + upgrade_section;
	}
	mUpgrade_window = new Window(dest, WINDOWS, WINDOWS);

	auto upgrade_costs = new Resources();
	upgrade_costs->set_resources(gConfig_file->value_or_zero(building_upgrade_section, "goldcosts"),
		gConfig_file->value_or_zero(building_upgrade_section, "woodcosts"),
		gConfig_file->value_or_zero(building_upgrade_section, "stonecosts"),
		gConfig_file->value_or_zero(building_upgrade_section, "ironcosts"),
		gConfig_file->value_or_zero(building_upgrade_section, "energycosts"),
		gConfig_file->value_or_zero(building_upgrade_section, "watercosts"),
		gConfig_file->value_or_zero(building_upgrade_section, "foodcosts"));

	SDL_Color text_color = { 0,0,0,0 };
	dest.x = mUpgrade_window->get_dim().x + 20;
	dest.y = mUpgrade_window->get_dim().y + mWindow->get_dim().h - 180;
	dest.w = 0;
	dest.h = 0;
	auto headline = new Text("Upgradecosts", dest, UPGRADEWINDOWCONTENT, text_color, mUpgrade_window);
	mUpgrade_window->add_text_to_window(headline);
	for (auto i = 0; i < RESOURCES_TOTAL; ++i)
	{
		dest.y += 20;
		auto resource_names = new Text(Resources::get_name(RESOURCETYPES(i)), dest, UPGRADEWINDOWCONTENT, text_color, this);
		mUpgrade_window->add_text_to_window(resource_names);
		auto upgrade_cost_values = new Text(std::to_string(upgrade_costs->get_resource(RESOURCETYPES(i))), dest, UPGRADEWINDOWCONTENT, text_color, this);
		upgrade_cost_values->add_x_dim(60);
		mUpgrade_window->add_text_to_window(upgrade_cost_values);
	}

	mUpgrade_window->set_rendering_enabled(false);
	mUpgrade_window->disable();
}

UpgradeButton::~UpgradeButton()
{
	delete mUpgrade_window;
}

std::string UpgradeButton::get_upgrade_section()
{
	return mUpgrade_section;
}

void UpgradeButton::on_mouse_over(int mouse_x, int mouse_y)
{
	mUpgrade_window->set_rendering_enabled(true);
	//mUpgrade_window->set_clicked(true);
}

void UpgradeButton::on_mouse_out(int mouse_x, int mouse_y)
{
	mUpgrade_window->set_rendering_enabled(false);
	//mUpgrade_window->set_clicked(false);
}

void UpgradeButton::on_click(int mouse_x, int mouse_y)
{
	WindowButton::on_click(mouse_x, mouse_y);
}


