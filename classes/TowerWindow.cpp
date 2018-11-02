#include "TowerWindow.h"
#include "Tower.h"

TowerWindow::TowerWindow(SDL_Rect dim, Tower* tower) : BuildingWindow(dim, tower)
{
	SDL_Rect button_dim;
	button_dim.x = dim.x + 170;
	button_dim.y = dim.y;
	button_dim.w = 26;
	button_dim.h = 26;
	mUpgrade_damage_button = new Button("testbutton", button_dim, this, TOWERWINDOWBUTTONIDS::UPGRADE_DAMAGE_BUTTON);
	button_dim.y += 30;
	mUpgrade_range_button = new Button("testbutton", button_dim, this, TOWERWINDOWBUTTONIDS::UPGRADE_RANGE_BUTTON);
	button_dim.y += 30;
	mUpgrade_attackspeed_button = new Button("testbutton", button_dim, this, TOWERWINDOWBUTTONIDS::UPGRADE_ATTACKSPEED_BUTTON);
	mBuilding = tower;
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
	dynamic_cast<Tower*>(mBuilding)->upgrade_damage();
}

void TowerWindow::upgrade_range()
{
	dynamic_cast<Tower*>(mBuilding)->upgrade_range();
}

void TowerWindow::upgrade_attackspeed()
{
	dynamic_cast<Tower*>(mBuilding)->upgrade_attackspeed();
}

void TowerWindow::on_button_press(int button_id)
{
	if (button_id == UPGRADE_DAMAGE_BUTTON) this->upgrade_damage();
	if (button_id == UPGRADE_RANGE_BUTTON) this->upgrade_range();
	if (button_id == UPGRADE_ATTACKSPEED_BUTTON) this->upgrade_attackspeed();
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

