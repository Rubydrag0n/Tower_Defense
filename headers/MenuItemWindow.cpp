#include "MenuItemWindow.h"


MenuItemWindow::MenuItemWindow(SDL_Rect dim, Ressources res) : Window(dim)
{
	this->mConstruction_costs = res;
}

MenuItemWindow::~MenuItemWindow()
{
	
}

void MenuItemWindow::render()
{
	Window::render();
}
