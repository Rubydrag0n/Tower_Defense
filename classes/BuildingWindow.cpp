#include "BuildingWindow.h"
#include "LayerHandler.h"
#include "Building.h"
#include "Enums.h"
#include <iostream>
#include "ConfigFile.h"
#include "UpgradeButton.h"

BuildingWindow::BuildingWindow(SDL_Rect dim, Building* building) : Window(dim), mBuilding(building)
{
	//correct position of the window, so it does not collide with the border
	if (building->get_coords().y < mDim.h/3)
	{
		mDim.y += mDim.h/3 - building->get_coords().y;
	}
	if (building->get_coords().y > 1024 - mDim.h * 2/3)
	{
		mDim.y += 1024 - building->get_coords().y - mDim.h * 2/3;
	}
	if (building->get_coords().x > 1280 - mDim.w)
	{
		mDim.x += -building->get_dimensions().w - mDim.w;
	}
	mBuilding = building;

	mText = new Text*[RESOURCES_TOTAL];
	
	SDL_Rect button_dim;
	mButton_offset.x = 160;
	mButton_offset.y = 70;
	button_dim.x = static_cast<int>(mDim.x + mButton_offset.x);
	button_dim.y = static_cast<int>(mDim.y + mButton_offset.y);
	button_dim.w = 26;
	button_dim.h = 26;

	mDemolish_button = new Button("testbutton", button_dim, this, this, BUILDINGWINDOWBUTTONIDS::DEMOLISH_BUTTON);

	update_great_upgrades();

	mText_color = { 0, 0, 0, 0 };
	SDL_Rect headline_dest;
	headline_dest.x = get_dim().x + 20;
	headline_dest.y = get_dim().y + 420;
	headline_dest.w = 0;	//setting these to 0 will not scale anything
	headline_dest.h = 0;

	mHeadline = new Text("Storage      Maintenance", headline_dest, WINDOWBUTTONS, mText_color, this);
	
	//initialize map for text lines
	for (auto i = 0; i < RESOURCES_TOTAL; ++i)
	{
		headline_dest.y += 20;
		mText[i] = new Text(Resources::get_name(RESOURCETYPES(i))
			+ ": \t"
			+ std::to_string(mBuilding->get_current_resources()->get_display_resources().get_resource(RESOURCETYPES(i)))
			+ "/" + std::to_string(mBuilding->get_current_resources()->get_limit()->get_resource(RESOURCETYPES(i)))
			+ "     " + std::to_string(mBuilding->get_maintenance()->get_resource(RESOURCETYPES(i))), 
			headline_dest, WINDOWS, mText_color, this);
	}
}

BuildingWindow::~BuildingWindow()
{
	delete mDemolish_button;
	for (auto& upgrade : mBig_upgrades)
	{
		delete upgrade;
	}

	for (auto i = 0; i < RESOURCES_TOTAL; i++)
	{
		delete mText[i];
	}
	delete[] mText;
}

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
	update_great_upgrades();
}

void BuildingWindow::show_more(Button* button)
{
	auto value_to_shift = 60; // y-direction-shift
	if (!mBig_upgrades.empty())
	{
		for (auto i = 0; i < mBig_upgrades.size(); i++)
		{
			auto big_upgrade = mBig_upgrades.at(i);
		
			if (big_upgrade->is_upgrade_description_shown()) big_upgrade->set_upgrade_description_shown(false);
			if (big_upgrade->get_show_more_button() == button) big_upgrade->set_upgrade_description_shown(true);

			if (big_upgrade->is_shifted_down())	big_upgrade->shift(-value_to_shift);
			if(big_upgrade->get_show_more_button()->get_dimension().y > button->get_dimension().y) big_upgrade->shift(value_to_shift);
		}
	}
}

void BuildingWindow::render()
{
	Window::render();
	for (auto i = 0; i < RESOURCES_TOTAL; ++i)
	{
		mText[i]->set_text(Resources::get_name(RESOURCETYPES(i))
			+ ": \t"
			+ std::to_string(mBuilding->get_current_resources()->get_display_resources().get_resource(RESOURCETYPES(i)))
			+ "/" + std::to_string(mBuilding->get_current_resources()->get_limit()->get_resource(RESOURCETYPES(i)))
			+ "     " + std::to_string(mBuilding->get_maintenance()->get_resource(RESOURCETYPES(i))));
	}
}

void BuildingWindow::update_great_upgrades()
{
	//first delete old buttons
	if(!mBig_upgrades.empty())
	{
		for (auto i = 0; i< mBig_upgrades.size(); i++)
		{
			delete mBig_upgrades.at(i);
		}
		mBig_upgrades.clear();
	}

	//then create new buttons
	SDL_Rect button_dim;
	button_dim.x = static_cast<int>(mDim.x + mButton_offset.x);
	button_dim.y = static_cast<int>(mDim.y + mButton_offset.y);
	button_dim.w = 26;
	button_dim.h = 26;
	
	for(auto i = 1; ; i++)
	{
		const auto gap_between_to_upgrades = 30;
		const auto y_difference = -160; // how much the Show-More-Button is away from the Big-Upgrade-Button on the y-Axis
		button_dim.y += gap_between_to_upgrades;
		auto upgrade_section = mBuilding->get_building_level() + std::to_string(i);
		if(!gConfig_file->value_exists(mBuilding->get_name() + "/upgrade" + upgrade_section, "exists"))
		{
			break;
		}
		auto big_upgrade_button = new UpgradeButton("testbutton", button_dim, this, this, upgrade_section, UPGRADE_BUTTON);
		auto show_more_button = new ShowMoreButton("testbutton", button_dim, this, this, SHOW_MORE_BUTTON);
		show_more_button->add_x_dimension(y_difference);
		show_more_button->set_clickable_space(show_more_button->get_dimension());
		auto big_upgrade = new BigUpgrade(mBuilding->get_name(), upgrade_section, big_upgrade_button, show_more_button);
		mBig_upgrades.push_back(big_upgrade);
	}
}

void BuildingWindow::on_button_press(const int button_id, Button* button)
{
	if (button_id == DEMOLISH_BUTTON) this->demolish_building();
	if (button_id == UPGRADE_BUTTON) this->upgrade_building(button);
	if (button_id == SHOW_MORE_BUTTON) this->show_more(button);
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

std::vector<BigUpgrade*> BuildingWindow::get_big_upgrades()
{
	return mBig_upgrades;
}
