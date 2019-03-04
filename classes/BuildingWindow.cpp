#include "BuildingWindow.h"
#include "LayerHandler.h"
#include "Building.h"
#include "Enums.h"

#include <iostream>
#include "ConfigFile.h"
#include "UpgradeButton.h"


void BuildingWindow::demolish_building() const
{
	mBuilding->demolish();
	delete mBuilding;
}

void BuildingWindow::upgrade_building(Button* button)
{
	const auto building_upgrade_section = mBuilding->get_name() + "/upgrade" + dynamic_cast<UpgradeButton*>(button)->get_upgrade_section();
	if (gConfig_file->value_or_zero(building_upgrade_section, "count_of_little_upgrades") > mBuilding->get_count_of_little_upgrades())
	{
		return;
	}
	mBuilding->upgrade(building_upgrade_section);
	mBuilding->set_building_level(dynamic_cast<UpgradeButton*>(button)->get_upgrade_section() + ".");
	update_great_upgrade_buttons();
}


BuildingWindow::BuildingWindow(SDL_Rect dim, Building* building) : Window(dim), mBuilding(building)
{
	//correct position of the window, so it does not collide with the border
	if (building->get_coords().y < 200)
	{
		mDim.y = building->get_coords().y + building->get_dimensions().h;
	}
	if (building->get_coords().x > 1080)
	{
		mDim.x = building->get_coords().x + 64 - 200;
	}
	mBuilding = building;

	SDL_Rect button_dim;
	mButton_offset.x = 0;
	mButton_offset.y = 0;
	button_dim.x = static_cast<int>(mDim.x + mButton_offset.x);
	button_dim.y = static_cast<int>(mDim.y + mButton_offset.y);
	button_dim.w = 26;
	button_dim.h = 26;

	mDemolish_button = new Button("testbutton", button_dim, this, BUILDINGWINDOWBUTTONIDS::DEMOLISH_BUTTON);

	update_great_upgrade_buttons();

	//initialize map for text lines
	for (auto i = 0; i < RESOURCES_TOTAL; ++i)
	{
		mText[RESOURCETYPES(i)] = new LTexture();
	}
	mHeadline = new LTexture();
}

BuildingWindow::~BuildingWindow()
{
	delete mDemolish_button;
	for (auto& button : mUpgrade_buttons)
	{
		delete button;
	}
}

void BuildingWindow::render()
{
	Window::render();

	SDL_Rect dest;
	dest.x = get_dim().x + 20;
	dest.y = get_dim().y + 20;
	dest.w = get_dim().w;
	dest.h = get_dim().h;
	const SDL_Color text_color = { 0, 0, 0, 0 };

	//render headline
	mHeadline->load_from_rendered_text("Storage", text_color);
	gLayer_handler->render_to_layer(mHeadline, WINDOWS, nullptr, &dest);

	//render all the single resource lines
	for (auto i = 0; i < RESOURCES_TOTAL; ++i)
	{
		mText[RESOURCETYPES(i)]->load_from_rendered_text(
			Resources::get_name(RESOURCETYPES(i)) 
				+ ": \t" 
				+ std::to_string(mBuilding->get_current_resources()->get_display_resources().get_resource(RESOURCETYPES(i))), 
			text_color);
		dest.y += 20;
		gLayer_handler->render_to_layer(mText[RESOURCETYPES(i)], WINDOWS, nullptr, &dest);
	}
}

void BuildingWindow::update_great_upgrade_buttons()
{
	//first delete old buttons
	if(!mUpgrade_buttons.empty())
	{
		for (auto& button : mUpgrade_buttons)
		{
			delete button;
		}
		mUpgrade_buttons.clear();
	}

	//then create new buttons
	SDL_Rect button_dim;
	button_dim.x = static_cast<int>(mDim.x + mButton_offset.x);
	button_dim.y = static_cast<int>(mDim.y + mButton_offset.y);
	button_dim.w = 26;
	button_dim.h = 26;
	
	for(auto i = 1; ; i++)
	{
		button_dim.y += 30;
		auto upgrade_section = mBuilding->get_building_level() + std::to_string(i);
		if(!gConfig_file->value_exists(mBuilding->get_name() + "/upgrade" + upgrade_section, "exists"))
		{
			break;
		}
		auto new_button = new UpgradeButton("testbutton", button_dim, this, upgrade_section, BUILDINGWINDOWBUTTONIDS::UPGRADE_BUTTON);
		mUpgrade_buttons.push_back(new_button);
	}
}

void BuildingWindow::on_button_press(const int button_id, Button* button)
{
	if (button_id == DEMOLISH_BUTTON) this->demolish_building();
	if (button_id == UPGRADE_BUTTON) this->upgrade_building(button);
}

Button* BuildingWindow::get_demolish_button() const
{
	return mDemolish_button;
}

CoordinatesInDouble BuildingWindow::get_button_offset() const
{
	return mButton_offset;
}

Building* BuildingWindow::get_building() const
{
	return mBuilding;
}

std::vector<Button*> BuildingWindow::get_upgrade_buttons()
{
	return mUpgrade_buttons;
}
