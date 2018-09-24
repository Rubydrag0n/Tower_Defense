#include "BuildingWindow.h"
#include "LayerHandler.h"
#include "Building.h"

void delete_building()
{
	
}

BuildingWindow::BuildingWindow(SDL_Rect dim, Building* building) : Window(dim)
{
	SDL_Rect button_dim;
	button_dim.x = dim.x;
	button_dim.y = dim.y;
	button_dim.w = 100;
	button_dim.h = 26;
	
	mDemolish_button = new Button("testbutton", button_dim, &delete_building);
}

BuildingWindow::~BuildingWindow()
{
	
}

void BuildingWindow::render()
{
	Window::render();
	/*SDL_Rect button_dim;
	button_dim.x = get_dim().x;
	button_dim.y = get_dim().y;
	button_dim.w = 100;
	button_dim.h = 26;
	mDemolish_button = nullptr;
	mDemolish_button = new Button("testbutton", button_dim, &delete_building);*/
	
}

void BuildingWindow::delete_button()
{
	mDemolish_button = nullptr;
}


