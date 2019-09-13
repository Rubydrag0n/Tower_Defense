#include "BuildingMenuItem.h"
#include "ConfigFile.h"
#include "MouseHandler.h"
#include "BuildingMouseItem.h"
#include "Menu.h"

BuildingMenuItem::BuildingMenuItem(const std::string& name_of_object, Level *level, const SDL_Point coords, LAYERS click_layer, LAYERS render_layer) : MenuItem(name_of_object, level, coords, click_layer, render_layer)
{
	//set the construction costs of the building
	const auto building_stats_section = name_of_object + "/stats";
	mConstruction_costs.set_resources(gConfig_file->value_or_zero(building_stats_section, "goldcosts"),
		gConfig_file->value_or_zero(building_stats_section, "woodcosts"),
		gConfig_file->value_or_zero(building_stats_section, "stonecosts"),
		gConfig_file->value_or_zero(building_stats_section, "ironcosts"),
		gConfig_file->value_or_zero(building_stats_section, "energycosts"),
		gConfig_file->value_or_zero(building_stats_section, "watercosts"),
		gConfig_file->value_or_zero(building_stats_section, "foodcosts"));

	auto storage_limit = new Resources();
	storage_limit->set_resources(gConfig_file->value_or_zero(building_stats_section, "goldLimit"),
		gConfig_file->value_or_zero(building_stats_section, "woodLimit"),
		gConfig_file->value_or_zero(building_stats_section, "stoneLimit"),
		gConfig_file->value_or_zero(building_stats_section, "ironLimit"),
		gConfig_file->value_or_zero(building_stats_section, "energyLimit"),
		gConfig_file->value_or_zero(building_stats_section, "waterLimit"),
		gConfig_file->value_or_zero(building_stats_section, "foodLimit"));

	auto maintenance = new Resources();
	maintenance->set_resources(gConfig_file->value_or_zero(building_stats_section, "goldMain"),
		gConfig_file->value_or_zero(building_stats_section, "woodMain"),
		gConfig_file->value_or_zero(building_stats_section, "stoneMain"),
		gConfig_file->value_or_zero(building_stats_section, "ironMain"),
		gConfig_file->value_or_zero(building_stats_section, "energyMain"),
		gConfig_file->value_or_zero(building_stats_section, "waterMain"),
		gConfig_file->value_or_zero(building_stats_section, "foodMain"));

	const std::string kind_of_object = gConfig_file->value(mName_of_object + "/menuitem", "kind_of_object");
	SDL_Color text_color = { 0, 0, 0 ,0 };
	SDL_Rect rect;
	rect.x = 1280;
	rect.y = 424;
	rect.w = 400;
	rect.h = 200;
	mMenu_item_window = new Window(rect, WINDOWS, WINDOWS);
	mMenu_item_window->set_rendering_enabled(false);
	mMenu_item_window->disable();
	rect.x += 20;
	rect.y += 20;
	rect.w = 0;//no scaling on text
	rect.h = 0;

	if(kind_of_object == "path")
	{
		auto speed_multiplier = gConfig_file->value(building_stats_section, "speed_multiplier");
		auto speed_multiplier_name = new Text("Speed Multi", rect, WINDOWCONTENT, text_color, mMenu_item_window);
		mMenu_item_window->add_text_to_window(speed_multiplier_name);
		auto speed_multiplier_value = new Text(std::to_string(speed_multiplier), rect, WINDOWCONTENT, text_color, mMenu_item_window);
		speed_multiplier_value->add_x_dim(120);
		mMenu_item_window->add_text_to_window(speed_multiplier_value);
	}
	else
	{
		auto headline = new Text("       MaxStorage   Mainten", rect, WINDOWCONTENT, text_color, mMenu_item_window);
		mMenu_item_window->add_text_to_window(headline);
		auto storage_values = new Text*[RESOURCES_TOTAL];
		auto maintenance_values = new Text*[RESOURCES_TOTAL];
		for (auto i = 0; i < RESOURCES_TOTAL; ++i)
		{
			rect.y += 20;
			storage_values[i] = new Text(std::to_string(storage_limit->get_resource(RESOURCETYPES(i))), rect, WINDOWCONTENT, text_color, mMenu_item_window);
			storage_values[i]->add_x_dim(60);
			mMenu_item_window->add_text_to_window(storage_values[i]);
			maintenance_values[i] = new Text(std::to_string(maintenance->get_resource(RESOURCETYPES(i))), rect, WINDOWCONTENT, text_color, mMenu_item_window);
			maintenance_values[i]->add_x_dim(130);
			mMenu_item_window->add_text_to_window(maintenance_values[i]);
			auto resource_names = new Text(Resources::get_name(RESOURCETYPES(i)), rect, WINDOWS, text_color, mMenu_item_window);
			mMenu_item_window->add_text_to_window(resource_names);
		}
		rect.x = mMenu_item_window->get_dim().x + 220;
		rect.y = mMenu_item_window->get_dim().y + 20;
		rect.w = 0;	//setting these to 0 will not scale anything
		rect.h = 0;


		if (kind_of_object == "homingtower" || kind_of_object == "aoetower")
		{
			auto damage = new Damage();
			damage->set_damages(gConfig_file->value_or_zero(building_stats_section, "phys"),
				gConfig_file->value_or_zero(building_stats_section, "magic"),
				gConfig_file->value_or_zero(building_stats_section, "fire"),
				gConfig_file->value_or_zero(building_stats_section, "water"),
				gConfig_file->value_or_zero(building_stats_section, "elec"));
			auto range = gConfig_file->value(building_stats_section, "range");
			auto attack_speed = gConfig_file->value(building_stats_section, "attackspeed");

			auto dmg_text = new Text("Dmg: ", rect, WINDOWS, text_color, mMenu_item_window);
			mMenu_item_window->add_text_to_window(dmg_text);
			rect.y += 30;
			auto as_text = new Text("AS: ", rect, WINDOWS, text_color, mMenu_item_window);
			mMenu_item_window->add_text_to_window(as_text);
			rect.y += 30;
			auto range_text = new Text("Range: ", rect, WINDOWS, text_color, mMenu_item_window);
			mMenu_item_window->add_text_to_window(range_text);
			rect.y += 30;
			auto damage_distribution_headline = new Text("Damage dist: ", rect, WINDOWS, text_color, mMenu_item_window);
			mMenu_item_window->add_text_to_window(damage_distribution_headline);
			rect.y += 30;
			auto damage_distribution_text = new Text("P: " + std::to_string(int(damage->get_phys_dmg()))
				+ " M: " + std::to_string(int(damage->get_magic_dmg()))
				+ " F: " + std::to_string(int(damage->get_fire_dmg()))
				+ " W: " + std::to_string(int(damage->get_water_dmg()))
				+ " E: " + std::to_string(int(damage->get_elec_dmg())), rect, WINDOWS, text_color, mMenu_item_window);
			mMenu_item_window->add_text_to_window(damage_distribution_text);
			rect.x = mMenu_item_window->get_dim().x + 280;
			rect.y = mMenu_item_window->get_dim().y + 20;
			auto dmg_value = new Text(std::to_string(int(damage->get_dmg_sum())), rect, WINDOWS, text_color, mMenu_item_window);
			mMenu_item_window->add_text_to_window(dmg_value);
			rect.y += 30;
			auto as_value = new Text(std::to_string(int(attack_speed)), rect, WINDOWS, text_color, mMenu_item_window);
			mMenu_item_window->add_text_to_window(as_value);
			rect.y += 30;
			auto range_value = new Text(std::to_string(int(range)), rect, WINDOWS, text_color, mMenu_item_window);
			mMenu_item_window->add_text_to_window(range_value);
			delete damage;
			if(kind_of_object == "aoetower")
			{
				auto explosive_radius = gConfig_file->value(building_stats_section, "explosiveradius");
				rect.x = mMenu_item_window->get_dim().x + 220;
				rect.y = mMenu_item_window->get_dim().y + 160;
				auto exlosive_radius_text = new Text("Explosive Radius: ", rect, WINDOWCONTENT, text_color, mMenu_item_window);
				mMenu_item_window->add_text_to_window(exlosive_radius_text);
				auto explosive_radius_value = new Text(std::to_string(int(explosive_radius)), rect, WINDOWCONTENT, text_color, mMenu_item_window);
				explosive_radius_value->add_x_dim(130);
				mMenu_item_window->add_text_to_window(explosive_radius_value);
			}
		}
		else if (kind_of_object == "industrialbuilding")
		{
			auto produce = new Resources();
			produce->set_resources(gConfig_file->value_or_zero(building_stats_section, "goldproduction"),
				gConfig_file->value_or_zero(building_stats_section, "woodproduction"),
				gConfig_file->value_or_zero(building_stats_section, "stoneproduction"),
				gConfig_file->value_or_zero(building_stats_section, "ironproduction"),
				gConfig_file->value_or_zero(building_stats_section, "energyproduction"),
				gConfig_file->value_or_zero(building_stats_section, "waterproduction"),
				gConfig_file->value_or_zero(building_stats_section, "foodproduction"));
			headline = new Text("Production", rect, WINDOWCONTENT, text_color, mMenu_item_window);
			mMenu_item_window->add_text_to_window(headline);
			auto production_values = new Text*[RESOURCES_TOTAL];
			for (auto i = 0; i < RESOURCES_TOTAL; ++i)
			{
				rect.y += 20;
				auto resource_names = new Text(Resources::get_name(RESOURCETYPES(i)), rect, WINDOWS, text_color, mMenu_item_window);
				mMenu_item_window->add_text_to_window(resource_names);
				production_values[i] = new Text(std::to_string(produce->get_display_resources().get_resource(RESOURCETYPES(i))), rect, WINDOWCONTENT, text_color, mMenu_item_window);
				production_values[i]->add_x_dim(60);
				mMenu_item_window->add_text_to_window(production_values[i]);
			}
			delete produce;
		}
		delete storage_limit;
		delete maintenance;
	}
}

BuildingMenuItem::~BuildingMenuItem() = default;

void BuildingMenuItem::render()
{
	MenuItem::render();
}

void BuildingMenuItem::on_click(int mouse_x, int mouse_y)
{
//	Building::get_default_stats(mName_of_object);
	gMouse_handler->set_item_on_mouse(new BuildingMouseItem(mName_of_object, mSprite, mLevel, mConstruction_costs, LAYERS::OVERLAY));
	mLevel->get_menu()->set_menu_item_window(mMenu_item_window);
}

Resources BuildingMenuItem::get_construction_costs() const
{
	return mConstruction_costs;
}
