#include "BuildingWindow.h"
#include "LayerHandler.h"
#include "Building.h"
#include "Enums.h"
#include <iostream>
#include "ConfigFile.h"
#include "UpgradeButton.h"
#include "ShowMoreButton.h"
#include "BigUpgrade.h"

BuildingWindow::BuildingWindow(SDL_Rect dim, Building* building) : Window(dim, WINDOWS, WINDOWS), mBuilding(building)
{
	mBuilding = building;
	mText_color = { 0, 0, 0, 0 };
	
	mResource_names = new Text*[RESOURCES_TOTAL];
	mStorage_values = new Text*[RESOURCES_TOTAL];
	mMaintenance_values = new Text*[RESOURCES_TOTAL];
	
	mButton_offset.x = 160;
	mButton_offset.y = 20;

	update_great_upgrades();

	SDL_Rect headline_dest;
	headline_dest.x = mDim.x + 20;
	headline_dest.y = mDim.y + 20;
	headline_dest.w = 0;	//setting these to 0 will not scale anything
	headline_dest.h = 0;
	//window for the warehouse is at a different position, than the other buildingwindows
	if(mBuilding->get_name() != "warehouse")
	{
		headline_dest.x += mDim.w - 200;
	}
	mHeadline = new Text("       Storage   Mainten", headline_dest, WINDOWCONTENT, mText_color, this);
	
	//initialize map for text lines
	for (auto i = 0; i < RESOURCES_TOTAL; ++i)
	{
		headline_dest.y += 20;
		mResource_names[i] = new Text(Resources::get_name(RESOURCETYPES(i)), headline_dest, WINDOWCONTENT, mText_color, this);
		mStorage_values[i] = new Text(std::to_string(mBuilding->get_current_resources()->get_display_resources().get_resource(RESOURCETYPES(i)))
			+ "/" + std::to_string(mBuilding->get_current_resources()->get_limit()->get_resource(RESOURCETYPES(i))), headline_dest, WINDOWCONTENT, mText_color, this);
		mStorage_values[i]->add_x_dim(60);
		mMaintenance_values[i] = new Text(std::to_string(mBuilding->get_maintenance()->get_resource(RESOURCETYPES(i))), headline_dest, WINDOWCONTENT, mText_color, this);
		mMaintenance_values[i]->add_x_dim(130);
	}
}

BuildingWindow::~BuildingWindow()
{
	for (auto& upgrade : mBig_upgrades)
	{
		delete upgrade;
	}

	for (auto i = 0; i < RESOURCES_TOTAL; i++)
	{
		delete mResource_names[i];
		delete mStorage_values[i];
		delete mMaintenance_values[i];
	}
	delete[] mResource_names;
	delete[] mStorage_values;
	delete[] mMaintenance_values;
	delete mHeadline;
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
	const auto value_to_shift = 60; // y-direction-shift
	if (!mBig_upgrades.empty())
	{
		for (auto big_upgrade : mBig_upgrades)
		{
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
		mResource_names[i]->set_text(Resources::get_name(RESOURCETYPES(i)));
		mStorage_values[i]->set_text(std::to_string(mBuilding->get_current_resources()->get_display_resources().get_resource(RESOURCETYPES(i)))
			+ "/" + std::to_string(mBuilding->get_current_resources()->get_limit()->get_resource(RESOURCETYPES(i))));
		mMaintenance_values[i]->set_text(std::to_string(mBuilding->get_maintenance()->get_resource(RESOURCETYPES(i))));
	}
}

void BuildingWindow::update_great_upgrades()
{
	//first delete old buttons
	if(!mBig_upgrades.empty())
	{
		for (auto& big_upgrade : mBig_upgrades)
		{
			delete big_upgrade;
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
		const auto upgrade_section = mBuilding->get_building_level() + std::to_string(i);
		if(!gConfig_file->value_exists(mBuilding->get_name() + "/upgrade" + upgrade_section, "exists"))
		{
			break;
		}
		const auto big_upgrade_button = new UpgradeButton("testbutton", button_dim, this, upgrade_section, WINDOWCONTENT, WINDOWCONTENT, this, UPGRADE_BUTTON);
		auto show_more_button = new ShowMoreButton("testbutton", button_dim, this, WINDOWCONTENT, WINDOWCONTENT, this, SHOW_MORE_BUTTON);
		show_more_button->add_x_dimension(y_difference);
		show_more_button->set_clickable_space(show_more_button->get_dimension());
		auto big_upgrade = new BigUpgrade(mBuilding->get_name(), upgrade_section, big_upgrade_button, show_more_button);
		mBig_upgrades.push_back(big_upgrade);
	}
}

void BuildingWindow::on_button_press(const int button_id, Button* button)
{
	if (button_id == UPGRADE_BUTTON) this->upgrade_building(button);
	if (button_id == SHOW_MORE_BUTTON) this->show_more(button);
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

void BuildingWindow::on_click(int mouse_x, int mouse_y)
{
	mBuilding->set_clicked(true);
}




