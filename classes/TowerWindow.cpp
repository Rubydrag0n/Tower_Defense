#include "TowerWindow.h"
#include "Tower.h"
#include <iostream>

TowerWindow::TowerWindow(SDL_Rect dim, Tower* tower) : BuildingWindow(dim, tower)
{
	SDL_Rect button_dim;
	button_dim.x = dim.x + 170;
	button_dim.y = dim.y;
	button_dim.w = 26;
	button_dim.h = 26;
	mUpgrade_damage_button = new Button("testbutton", button_dim, this, BUILDINGWINDOWBUTTONIDS::UPGRADE_DAMAGE_BUTTON);
	button_dim.y += 30;
	mUpgrade_range_button = new Button("testbutton", button_dim, this, BUILDINGWINDOWBUTTONIDS::UPGRADE_RANGE_BUTTON);
	button_dim.y += 30;
	mUpgrade_attackspeed_button = new Button("testbutton", button_dim, this, BUILDINGWINDOWBUTTONIDS::UPGRADE_ATTACKSPEED_BUTTON);
}

TowerWindow::~TowerWindow()
{
	delete mUpgrade_damage_button;
	delete mUpgrade_range_button;
	delete mUpgrade_attackspeed_button;
}

void TowerWindow::render()
{
	BuildingWindow::render();
}

void TowerWindow::upgrade_damage()
{
	std::cout << "dmg";
	dynamic_cast<Tower*>(mBuilding)->upgrade_damage();
}

void TowerWindow::upgrade_range()
{
	std::cout << "range";
	dynamic_cast<Tower*>(mBuilding)->upgrade_range();
}

void TowerWindow::upgrade_attackspeed()
{
	std::cout << "speed";
	dynamic_cast<Tower*>(mBuilding)->upgrade_attackspeed();
}

void TowerWindow::on_button_press(int button_id)
{
	if (button_id == UPGRADE_DAMAGE_BUTTON) this->upgrade_damage();
	if (button_id == UPGRADE_RANGE_BUTTON) this->upgrade_range();
	if (button_id == UPGRADE_ATTACKSPEED_BUTTON) this->upgrade_attackspeed();
	BuildingWindow::on_button_press(button_id);
}

Button* TowerWindow::get_upgrade_damage_button()
{
	return mUpgrade_damage_button;
}

Button* TowerWindow::get_upgrade_range_button()
{
	return mUpgrade_range_button;
}

Button* TowerWindow::get_upgrade_attackspeed_button()
{
	return mUpgrade_attackspeed_button;
}

