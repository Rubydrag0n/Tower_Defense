#pragma once
#include "Tower.h"

class AoeTower final : 
	public Tower
{
public:
	AoeTower(const std::string& tower_name, SDL_Point coords, Level* level, LAYERS click_layer, LAYERS render_layer);

	void create_shot(Enemy* enemy) override;
	void on_tick() override;
	void update_building_window() override;
	void set_stat_strings_for_upgrade_buttons(UpgradeButton* button) override;
	bool upgrade(const std::string& tower_upgrade_section) override; 

private:
	double mExplosive_radius;

	Text* mExplosive_radius_value;
};
