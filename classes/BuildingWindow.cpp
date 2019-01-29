#include "BuildingWindow.h"
#include "LayerHandler.h"
#include "Building.h"
#include <iostream>
#include "SDL_setup.h"

void BuildingWindow::demolish_building()
{
	mBuilding->demolish();
	std::cout << "dem";
	delete mBuilding;
}

void BuildingWindow::upgrade_building()
{
	std::cout << "up";
	auto building_upgrade_section = mBuilding->get_name() + "/upgrade" + std::to_string(mBuilding->get_building_level()+1);
	mBuilding->upgrade(building_upgrade_section);
}


BuildingWindow::BuildingWindow(SDL_Rect dim, Building* building) : Window(dim)
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
	
	SDL_Rect button_dim;
	mButton_offset.x = 0;
	mButton_offset.y = 140;
	button_dim.x = dim.x + mButton_offset.x;
	button_dim.y = dim.y + mButton_offset.y;
	button_dim.w = 26;
	button_dim.h = 26;
	mDemolish_button = new Button("testbutton", button_dim, this, BUILDINGWINDOWBUTTONIDS::DEMOLISH_BUTTON);
	button_dim.y += 20;
	mUpgrade_button = new Button("testbutton", button_dim, this, BUILDINGWINDOWBUTTONIDS::UPGRADE_BUTTON);
	if(building->get_building_max_level() == 0)
	{
		mUpgrade_button->disable();
		mUpgrade_button->set_rendering_enabled(false);
	}
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
	SDL_Color text_color = { 0, 0, 0 };

	auto maintenace_heading = new LTexture();
	maintenace_heading->load_from_rendered_text("Maintenance", text_color);
	gLayer_handler->render_to_layer(maintenace_heading, LAYERS::WINDOWS, nullptr, &dest);

	auto gold_line = new LTexture();
	gold_line->load_from_rendered_text("Gold: " + std::to_string(mBuilding->get_maintenance()->get_resource(RESOURCETYPES::GOLD)), text_color);
	dest.y += 20;
	gLayer_handler->render_to_layer(gold_line, LAYERS::WINDOWS, nullptr, &dest);

	auto food_line = new LTexture();
	food_line->load_from_rendered_text("Food: " + std::to_string(mBuilding->get_maintenance()->get_resource(RESOURCETYPES::FOOD)), text_color);
	dest.y += 20;
	gLayer_handler->render_to_layer(food_line, LAYERS::WINDOWS, nullptr, &dest);

	auto wood_line = new LTexture();
	wood_line->load_from_rendered_text("Wood: " + std::to_string(mBuilding->get_maintenance()->get_resource(RESOURCETYPES::WOOD)), text_color);
	dest.y += 20;
	gLayer_handler->render_to_layer(wood_line, LAYERS::WINDOWS, nullptr, &dest);

	auto stone_line = new LTexture();
	stone_line->load_from_rendered_text("Stone: " + std::to_string(mBuilding->get_maintenance()->get_resource(RESOURCETYPES::STONE)), text_color);
	dest.y += 20;
	gLayer_handler->render_to_layer(stone_line, LAYERS::WINDOWS, nullptr, &dest);

	auto iron_line = new LTexture();
	iron_line->load_from_rendered_text("Iron: " + std::to_string(mBuilding->get_maintenance()->get_resource(RESOURCETYPES::IRON)), text_color);
	dest.y += 20;
	gLayer_handler->render_to_layer(iron_line, LAYERS::WINDOWS, nullptr, &dest);

	auto energy_line = new LTexture();
	energy_line->load_from_rendered_text("Energy: " + std::to_string(mBuilding->get_maintenance()->get_resource(RESOURCETYPES::ENERGY)), text_color);
	dest.y += 20;
	gLayer_handler->render_to_layer(energy_line, LAYERS::WINDOWS, nullptr, &dest);
}

void BuildingWindow::on_button_press(int button_id)
{
	if (button_id == DEMOLISH_BUTTON) this->demolish_building();
	if (button_id == UPGRADE_BUTTON) this->upgrade_building();
}

Button* BuildingWindow::get_demolish_button()
{
	return mDemolish_button;
}

Button* BuildingWindow::get_upgrade_button()
{
	return mUpgrade_button;
}


CoordinatesInDouble BuildingWindow::get_button_offset()
{
	return mButton_offset;
}

Building* BuildingWindow::get_building()
{
	return mBuilding;
}

