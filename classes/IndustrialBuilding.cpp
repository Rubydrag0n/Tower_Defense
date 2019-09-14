#include "IndustrialBuilding.h"
#include "ConfigFile.h"
#include "SDL_setup.h"
#include "Carriage.h"

IndustrialBuilding::IndustrialBuilding(std::string industrial_building_name, const SDL_Point coords, Level* level, LAYERS click_layer, LAYERS render_layer) : Building(
	std::move(industrial_building_name), coords, level, click_layer, render_layer)
{
	mCarriage = new Carriage("carriage", mLevel, ENEMIES, this, reinterpret_cast<Building*>(mLevel->get_main_building()));

	//add production text in the building window
	SDL_Color text_color = { 0,0,0,0 };
	SDL_Rect dest;
	dest.h = 0;
	dest.w = 0;
	dest.x = mBuilding_window->get_dim().x + 20;
	dest.y = mBuilding_window->get_dim().y + 30;
	auto const headline = new Text("        Production", dest, WINDOWCONTENT, text_color, mBuilding_window);
	mBuilding_window->add_text_to_window(headline);
	mProduction_values = new Text*[RESOURCES_TOTAL];
	for (auto i = 0; i < RESOURCES_TOTAL; ++i)
	{
		dest.y += 20;
		auto const resource_names = new Text(Resources::get_name(RESOURCETYPES(i)), dest, WINDOWCONTENT, text_color, mBuilding_window);
		mBuilding_window->add_text_to_window(resource_names);
		mProduction_values[i] = new Text(std::to_string(mProduce->get_display_resources().get_resource(RESOURCETYPES(i))), dest, WINDOWCONTENT, text_color, mBuilding_window);
		mProduction_values[i]->add_x_dim(60);
		mBuilding_window->add_text_to_window(mProduction_values[i]);
	}
}

void IndustrialBuilding::update_building_window(bool is_a_button_hovered)
{
	Building::update_building_window(is_a_button_hovered);
	for(auto i = 0; i < RESOURCES_TOTAL; ++i)
	{
		mProduction_values[i]->set_text(std::to_string(mProduce->get_display_resources().get_resource(RESOURCETYPES(i))));
	}
}

void IndustrialBuilding::on_tick()
{
	Building::on_tick();
	update_building_window(false);
}


BUILDINGTYPE IndustrialBuilding::get_building_type()
{
	return INDUSTRIAL_BUILDING;
}

void IndustrialBuilding::on_click(int mouse_x, int mouse_y)
{
	mLevel->get_menu()->set_building_window(mBuilding_window);
	Building::on_click(mouse_x, mouse_y);
}
