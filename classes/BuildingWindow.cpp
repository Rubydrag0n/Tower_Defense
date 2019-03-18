#include "BuildingWindow.h"
#include "LayerHandler.h"
#include "Building.h"
#include "Enums.h"
#include <iostream>
#include "ConfigFile.h"
#include "UpgradeButton.h"
#include "BigUpgradeButton.h"

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

	mText = new LTexture*[RESOURCES_TOTAL];
	

	SDL_Rect button_dim;
	mButton_offset.x = 150;
	mButton_offset.y = 110;
	button_dim.x = static_cast<int>(mDim.x + mButton_offset.x);
	button_dim.y = static_cast<int>(mDim.y + mButton_offset.y);
	button_dim.w = 26;
	button_dim.h = 26;

	mDemolish_button = new Button("testbutton", button_dim, this, BUILDINGWINDOWBUTTONIDS::DEMOLISH_BUTTON);

	update_great_upgrade_buttons();


	
	//initialize map for text lines
	for (auto i = 0; i < RESOURCES_TOTAL; ++i)
	{
		mText[i] = new LTexture();
	}

	mText_color = { 0, 0, 0, 0 };

	mHeadline = new LTexture();
	mHeadline->load_from_rendered_text("Storage      Maintenance", mText_color);
}

BuildingWindow::~BuildingWindow()
{
	delete mDemolish_button;
	for (auto& button : mUpgrade_buttons)
	{
		delete button;
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
	update_great_upgrade_buttons();
}

void BuildingWindow::render()
{
	Window::render();

	SDL_Rect dest;
	dest.x = get_dim().x + 20;
	dest.y = get_dim().y + 420;
	dest.w = 0;	//setting these to 0 will not scale anything
	dest.h = 0;

	//render headline
	gLayer_handler->render_to_layer(mHeadline, WINDOWS, nullptr, &dest);

	//render all the single resource lines
	for (auto i = 0; i < RESOURCES_TOTAL; ++i)
	{
		mText[i]->load_from_rendered_text(
			Resources::get_name(RESOURCETYPES(i)) 
				+ ": \t" 
				+ std::to_string(mBuilding->get_current_resources()->get_display_resources().get_resource(RESOURCETYPES(i))) 
				+ "/" + std::to_string(mBuilding->get_current_resources()->get_limit()->get_resource(RESOURCETYPES(i)))
				+ "     " + std::to_string(mBuilding->get_maintenance()->get_resource(RESOURCETYPES(i))), 
			mText_color);
		dest.y += 20;
		gLayer_handler->render_to_layer(mText[i], WINDOWS, nullptr, &dest);
	}
}

void BuildingWindow::update_great_upgrade_buttons()
{
	//first delete old buttons
	if(!mUpgrade_buttons.empty())
	{
		for (auto i = 0; i< mUpgrade_buttons.size(); i++)
		{
			delete mUpgrade_buttons.at(i);
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
		auto button = new BigUpgradeButton(mBuilding->get_name(), "testbutton", button_dim, this, upgrade_section, UPGRADE_BUTTON);
		mUpgrade_buttons.push_back(button);
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

std::vector<UpgradeButton*> BuildingWindow::get_upgrade_buttons()
{
	return mUpgrade_buttons;
}
