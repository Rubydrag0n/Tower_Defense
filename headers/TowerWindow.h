#pragma once
#include "BuildingWindow.h"

class Tower;

enum TOWERWINDOWBUTTONIDS
{
	UPGRADE_DAMAGE_BUTTON,
	UPGRADE_RANGE_BUTTON,
	UPGRADE_ATTACKSPEED_BUTTON,
	BUTTON_COUNT
};

class TowerWindow : public BuildingWindow
{
public:
	TowerWindow(SDL_Rect dim, Tower* tower);
	~TowerWindow();

	void render() override;
	void on_button_press(int button_id) override;

	void upgrade_damage();
	void upgrade_range();
	void upgrade_attackspeed();

	Button* get_upgrade_damage_button();
	Button* get_upgrade_range_button();
	Button* get_upgrade_attackspeed_button();

private:
	Button* mUpgrade_damage_button;
	Button* mUpgrade_range_button;
	Button* mUpgrade_attackspeed_button;
};
