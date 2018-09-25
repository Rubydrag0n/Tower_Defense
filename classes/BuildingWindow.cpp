#include "BuildingWindow.h"
#include "LayerHandler.h"
#include "Building.h"

void BuildingWindow::demolish_building()
{
	mBuilding->demolish();
	delete mBuilding;
}

BuildingWindow::BuildingWindow(SDL_Rect dim, Building* building) : Window(dim)
{
	SDL_Rect button_dim;
	button_dim.x = dim.x;
	button_dim.y = dim.y;
	button_dim.w = 100;
	button_dim.h = 26;
	mBuilding = building;
	mDemolish_button = new Button("testbutton", button_dim, this, BUILDINGWINDOWBUTTONIDS::DEMOLISH_BUTTON);
}

BuildingWindow::~BuildingWindow()
{
	delete mDemolish_button;
}

void BuildingWindow::render()
{
	Window::render();	
}

void BuildingWindow::on_button_press(int button_id)
{
	if (button_id == DEMOLISH_BUTTON) {
		this->demolish_building();
	}
}

Button* BuildingWindow::get_demolish_button()
{
	return mDemolish_button;
}


