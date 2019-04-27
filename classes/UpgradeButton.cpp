#include "UpgradeButton.h"
#include "ConfigFile.h"
#include "LayerHandler.h"
#include "Building.h"

UpgradeButton::UpgradeButton(const std::string& button_name, SDL_Rect dim, ButtonObject* obj, std::string upgrade_section, LAYERS click_layer, LAYERS render_layers, BuildingWindow* window, int button_id) : WindowButton(button_name, dim, obj, click_layer, render_layers, window, button_id), mUpgrade_section(upgrade_section)
{
	SDL_Rect dest;
	dest.x = mButton_dimensions.x + mButton_dimensions.w;
	dest.y = mButton_dimensions.y;
	dest.w = 200;
	dest.h = 200;
	const auto building_upgrade_section = dynamic_cast<BuildingWindow*>(mWindow)->get_building()->get_name() + "/upgrade" + upgrade_section;
	mUpgrade_window = new UpgradeWindow(dest, building_upgrade_section);
	mUpgrade_window->set_rendering_enabled(false);
	mUpgrade_window->disable();

}

UpgradeButton::~UpgradeButton()
{
	
}

std::string UpgradeButton::get_upgrade_section()
{
	return mUpgrade_section;
}

void UpgradeButton::on_mouse_over(int mouse_x, int mouse_y)
{
	mUpgrade_window->set_rendering_enabled(true);
	mUpgrade_window->set_clicked(true);
}

void UpgradeButton::on_mouse_out(int mouse_x, int mouse_y)
{
	mUpgrade_window->set_rendering_enabled(false);
	mUpgrade_window->set_clicked(false);
}

