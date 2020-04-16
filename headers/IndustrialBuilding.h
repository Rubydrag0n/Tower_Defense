#pragma once
#include "Building.h"
#include "Level.h"

class IndustrialBuilding :
	public Building
{
public:
	IndustrialBuilding(std::string industrial_building_name, SDL_Point coords, Level* level, LAYERS click_layer, LAYERS render_layer);

	BUILDINGTYPE get_building_type() override;
	void on_click(int mouse_x, int mouse_y) override;
	void on_tick() override;
	void update_building_window() override;
	std::shared_ptr<Window> create_window() override;
	
private:
	Text** mProduction_values;
};
