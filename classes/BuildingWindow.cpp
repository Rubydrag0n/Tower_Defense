#include "BuildingWindow.h"
#include "LayerHandler.h"
#include "Building.h"

void demolish_building()
{
	
}

BuildingWindow::BuildingWindow(SDL_Rect dim, Building* building) : Window(dim)
{
	SDL_Rect button_dim;
	button_dim.x = dim.x;
	button_dim.y = dim.y;
	button_dim.w = 100;
	button_dim.h = 26;
	
	mDemolish_button = new Button("testbutton", button_dim, &demolish_building);
}

BuildingWindow::~BuildingWindow()
{
	
}

void BuildingWindow::render()
{
	Window::render();	
}

Button* BuildingWindow::get_demolish_button()
{
	return mDemolish_button;
}


