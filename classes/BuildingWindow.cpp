#include "BuildingWindow.h"
#include "LayerHandler.h"
#include "Building.h"
#include "Enums.h"

#include <iostream>


void BuildingWindow::demolish_building() const
{
	mBuilding->demolish();
	std::cout << "dem";
	delete mBuilding;
}

void BuildingWindow::upgrade_building() const
{
	std::cout << "up";
	const auto building_upgrade_section = mBuilding->get_name() + "/upgrade" + std::to_string(mBuilding->get_building_level()+1);
	mBuilding->upgrade(building_upgrade_section);
}


BuildingWindow::BuildingWindow(SDL_Rect dim, Building* building) : Window(dim), mBuilding(building)
{
	//correct position of the window, so it does not collide with the border
	dim.x = building->get_coords().x;
	dim.y = building->get_coords().y - 200;
	if (building->get_coords().y < 200)
	{
		dim.y = building->get_coords().y + building->get_dimensions().h;
	}
	if (building->get_coords().x > 1080)
	{
		dim.x = building->get_coords().x + 64 - 200;
	}
	dim.w = 200;
	dim.h = 200;
	set_dim(dim);
	mBuilding = building;

	SDL_Rect button_dim;
	mButton_offset.x = 0;
	mButton_offset.y = 140;
	button_dim.x = static_cast<int>(dim.x + mButton_offset.x);
	button_dim.y = static_cast<int>(dim.y + mButton_offset.y);
	button_dim.w = 26;
	button_dim.h = 26;

	std::string upgrade_section;
	/*for(auto i = 1; ; i++)
	{
		upgrade_section = std::to_string(i);
		if(!gConfig_file->value_exists(mBuilding->get_name() + "/upgrade" + upgrade_section, "exists"))
		{
			break;
		}
		auto new_button = new Button("testbutton", button_dim, this, BUILDINGWINDOWBUTTONIDS::UPGRADE_BUTTON);
		mUpgrade_buttons.insert(std::make_pair(new_button, upgrade_section));
	}*/
	mUpgrade_button = new Button("testbutton", button_dim, this, BUILDINGWINDOWBUTTONIDS::UPGRADE_BUTTON);
	button_dim.y += 20;
	mDemolish_button = new Button("testbutton", button_dim, this, BUILDINGWINDOWBUTTONIDS::DEMOLISH_BUTTON);
	mUpgrade_button->disable();
	mUpgrade_button->set_rendering_enabled(false);
	if(building->get_building_max_level() == 0)
	{
		mUpgrade_button->disable();
		mUpgrade_button->set_rendering_enabled(false);
	}

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
	delete mUpgrade_button;
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
				+ std::to_string(mBuilding->get_current_resources()->get_resource(RESOURCETYPES(i))), 
			text_color);
		dest.y += 20;
		gLayer_handler->render_to_layer(mText[RESOURCETYPES(i)], WINDOWS, nullptr, &dest);
	}
}

void BuildingWindow::on_button_press(const int button_id)
{
	if (button_id == DEMOLISH_BUTTON) this->demolish_building();
	if (button_id == UPGRADE_BUTTON) this->upgrade_building();
}

Button* BuildingWindow::get_demolish_button() const
{
	return mDemolish_button;
}

Button* BuildingWindow::get_upgrade_button() const
{
	return mUpgrade_button;
}


CoordinatesInDouble BuildingWindow::get_button_offset() const
{
	return mButton_offset;
}

Building* BuildingWindow::get_building() const
{
	return mBuilding;
}

