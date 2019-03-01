#pragma once
#include "BuildingWindow.h"

class Tower;

class TowerWindow : public BuildingWindow
{
public:
	TowerWindow(SDL_Rect dim, Tower* tower);
	~TowerWindow();

	void render() override;
	void on_button_press(int button_id) override;

	void upgrade_damage() const;
	void upgrade_range() const;
	void upgrade_attackspeed() const;

	Button* get_upgrade_damage_button() const;
	Button* get_upgrade_range_button() const;
	Button* get_upgrade_attackspeed_button() const;

private:
	Button* mUpgrade_damage_button;
	Button* mUpgrade_range_button;
	Button* mUpgrade_attackspeed_button;
};
