#include <SDL.h>
#include <functional>
#include "Menu.h"
#include "LayerHandler.h"
#include "ConfigFile.h"
#include "SDL_setup.h"
#include "DemolishTool.h"
#include "BuildingMenuItem.h"
#include "Building.h"

Menu::Menu(Level *level, const LAYERS render_layer) : Renderable(render_layer), mBuilding_window(nullptr), mUnit_window(nullptr)
{
	this->mLevel = level;
	this->mOpen_tab = TOWER;
	SDL_Rect dim;
	dim.x = 1300;
	dim.y = 0;
	dim.w = 100;
	dim.h = 40;
	mButtons[BUILDINGTYPE::TOWER]
		= new Button("TowerButton", dim, this, BUILDINGS, BUILDINGS, BUILDINGTYPE::TOWER);

	dim.x = 1400;
	mButtons[BUILDINGTYPE::INDUSTRIAL_BUILDING]
		= new Button("ResourceButton", dim, this, BUILDINGS, BUILDINGS, BUILDINGTYPE::INDUSTRIAL_BUILDING);

	dim.x = 1500;
	mButtons[BUILDINGTYPE::WAREHOUSE]
		= new Button("BuildingsButton", dim, this, BUILDINGS, BUILDINGS, BUILDINGTYPE::WAREHOUSE);

	dim.x = 1600;
	mButtons[BUILDINGTYPE::STREET]
		= new Button("TowerButton", dim, this, BUILDINGS, BUILDINGS, BUILDINGTYPE::STREET);


	for (auto i = 0; i < BUILDINGTYPE::BUILDINGTYPES_TOTAL; i++) {
		this->mBuilding_menu_items[BUILDINGTYPE(i)] = new std::vector<BuildingMenuItem*>();
	}

	this->sort_items_into_menu();
	this->show_tab(BUILDINGTYPE::TOWER);

	const SDL_Color text_color = { 0, 0, 255, 0 };
	SDL_Rect dest;
	dest.x = 0;
	dest.y = 1050;
	dest.w = 0;	//setting to 0 doesn't scale anything
	dest.h = 0;
	mMenu_texture = new Text("lives: " + std::to_string(mLevel->get_lives()) +
		" \tgold: " + std::to_string(mLevel->get_resources()->get_display_resources().get_resource(GOLD)) +
		" \twood: " + std::to_string(mLevel->get_resources()->get_display_resources().get_resource(WOOD)) +
		" \tstone: " + std::to_string(mLevel->get_resources()->get_display_resources().get_resource(STONE)) +
		" \tiron: " + std::to_string(mLevel->get_resources()->get_display_resources().get_resource(IRON)) +
		" \tenergy: " + std::to_string(mLevel->get_resources()->get_display_resources().get_resource(ENERGY)) +
		" \twater: " + std::to_string(mLevel->get_resources()->get_display_resources().get_resource(WATER)) +
		" \tfood: " + std::to_string(mLevel->get_resources()->get_display_resources().get_resource(FOOD)),
		dest, WINDOWS, text_color, nullptr);

	SDL_Point coords;
	coords.x = 1800;
	coords.y = 100;
	mDemolish_tool = new DemolishTool(mLevel, coords);
}

Menu::~Menu()
{
	//TODO: fix this deconstructor, it's broken
	//delete all the buttons and menu items
	/*for (auto i = 0; i < BUILDINGTYPES_TOTAL; i++) {
		delete mButtons[BUILDINGTYPE(i)];
		for (auto j = mMenu_items[BUILDINGTYPE(i)]->size(); j > 0; j--) {
			delete mMenu_items[BUILDINGTYPE(i)]->at(j);
		}
		mMenu_items[BUILDINGTYPE(i)]->clear();
		delete mMenu_items[BUILDINGTYPE(i)];
	}*/
	delete mBuilding_window;
	delete mUnit_window;
	delete mDemolish_tool;
}

void Menu::show_tab(const BUILDINGTYPE open_tab)
{
	this->mOpen_tab = open_tab;

	//enable all the items in the opened tab, disable all the items in the not opened tabs
	for (auto j = 0; j < BUILDINGTYPES_TOTAL; j++) {
		if (BUILDINGTYPE(j) == open_tab) {
			for (auto& i : *mBuilding_menu_items[open_tab])
			{
				i->set_rendering_enabled(true);
				i->enable();
			}
		}
		else
		{
			for (unsigned i = 0; i < mBuilding_menu_items[BUILDINGTYPE(j)]->size(); i++)
			{
				mBuilding_menu_items[BUILDINGTYPE(j)]->at(i)->set_rendering_enabled(false);
				mBuilding_menu_items[BUILDINGTYPE(j)]->at(i)->disable();
			}
		}
	}
}

void Menu::on_button_press(int button_id, Button* button)
{
	this->show_tab(BUILDINGTYPE(button_id));
}

void Menu::sort_items_into_menu()
{
	//TODO: So many magic numbers here...

	SDL_Point coords;
	std::string name_of_object;
	const auto number_of_items_per_row = 5;

	std::vector<std::string> types;
	types.emplace_back("tower");
	types.emplace_back("industrialbuildings");
	types.emplace_back("logisticsbuildings");
	types.emplace_back("streets");

	for (auto j = 0; j < BUILDINGTYPE::BUILDINGTYPES_TOTAL; j++) {
		for (auto i = 0; ; i++)
		{
			coords.x = 1300 + (i % number_of_items_per_row) * MENU_ITEM_WIDTH;
			coords.y = 64 + (i / number_of_items_per_row) * MENU_ITEM_HEIGHT;

			if(!gConfig_file->value_exists(types.at(j), std::to_string(i)))
			{
				break;
			}
			name_of_object.assign(gConfig_file->value(types.at(j), std::to_string(i)));
			const auto new_item = new BuildingMenuItem(name_of_object, mLevel, coords, WINDOWCONTENT, WINDOWCONTENT);
			this->add_menu_item(new_item, BUILDINGTYPE(j));
		}
	}
}

void Menu::render()
{

}

void Menu::add_menu_item(BuildingMenuItem* building_menu_item, const BUILDINGTYPE tab)
{
	this->mBuilding_menu_items[tab]->push_back(building_menu_item);
}

void Menu::set_building_window(Window* building_window)
{
	if (mBuilding_window != nullptr)
	{
		delete mBuilding_window;
	}
	mBuilding_window = building_window;
	mBuilding_window->set_rendering_enabled(true);
}

Window* Menu::get_building_window() const
{
	return mBuilding_window;
}

void Menu::set_unit_window(Window* unit_window)
{
	if (mUnit_window != nullptr) delete mUnit_window;
	mUnit_window = unit_window;
	mUnit_window->set_rendering_enabled(true);
}

Window* Menu::get_unit_window() const
{
	return mUnit_window;
}




