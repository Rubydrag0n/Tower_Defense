#include "BuildingMenuItem.h"
#include "ConfigFile.h"
#include "MouseHandler.h"
#include "BuildingMouseItem.h"

BuildingMenuItem::BuildingMenuItem(const std::string& name_of_object, Level *level, const SDL_Point coords, LAYERS click_layer, LAYERS render_layer) : MenuItem(name_of_object, level, coords, click_layer, render_layer)
{
	//set the construction costs of the building
	mConstruction_costs.set_resources(gConfig_file->value_or_zero(name_of_object + "/stats", "goldcosts"),
		gConfig_file->value_or_zero(name_of_object + "/stats", "woodcosts"),
		gConfig_file->value_or_zero(name_of_object + "/stats", "stonecosts"),
		gConfig_file->value_or_zero(name_of_object + "/stats", "ironcosts"),
		gConfig_file->value_or_zero(name_of_object + "/stats", "energycosts"),
		gConfig_file->value_or_zero(name_of_object + "/stats", "watercosts"),
		gConfig_file->value_or_zero(name_of_object + "/stats", "foodcosts"));
}

BuildingMenuItem::~BuildingMenuItem()
{
	
}

void BuildingMenuItem::render()
{
	MenuItem::render();
}

void BuildingMenuItem::on_click(int mouse_x, int mouse_y)
{
	gMouse_handler->set_item_on_mouse(new BuildingMouseItem(mName_of_object, mSprite, mLevel, mConstruction_costs, LAYERS::OVERLAY));
}

Resources BuildingMenuItem::get_construction_costs() const
{
	return mConstruction_costs;
}
