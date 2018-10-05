#include <SDL.h>
#include <functional>

#include "Menu.h"
#include "LTexture.h"
#include "LayerHandler.h"
#include "ConfigFile.h"
#include "SDL_setup.h"

Menu::Menu(Level *level)
{
	this->mLevel = level;
	this->mMenu_texture = nullptr;
	this->mOpen_tab = TOWER;
	SDL_Rect dim;
	dim.x = 1300;
	dim.y = 0;
	dim.w = 100;
	dim.h = 40;
	mButtons[BUILDINGTYPE::TOWER]
		= new Button("TowerButton", dim, this, BUILDINGTYPE::TOWER);

	dim.x = 1400;
	mButtons[BUILDINGTYPE::INDUSTRIAL_BUILDING]
		= new Button("TowerButton", dim, this, BUILDINGTYPE::INDUSTRIAL_BUILDING);

	dim.x = 1500;
	mButtons[BUILDINGTYPE::LOGISTICS_BUILDING]
		= new Button("TowerButton", dim, this, BUILDINGTYPE::LOGISTICS_BUILDING);

	for (auto i = 0; i < BUILDINGTYPE::TYPES_TOTAL; i++) {
		this->mMenu_items[BUILDINGTYPE(i)] = new std::vector<MenuItem*>();
	}

	this->sort_items_into_menu();
	this->show_tab(BUILDINGTYPE::TOWER);
}

Menu::~Menu()
{
	//TODO: fix this deconstructor, it's broken
	//delete all the buttons and menuitems
	for (auto i = 0; i < BUILDINGTYPE::TYPES_TOTAL; i++) {
		delete mButtons[BUILDINGTYPE(i)];
		for (auto j = mMenu_items[BUILDINGTYPE(i)]->size(); j > 0; j--) {
			delete (mMenu_items[BUILDINGTYPE(i)]->at(j));
		}
		mMenu_items[BUILDINGTYPE(i)]->clear();
		delete mMenu_items[BUILDINGTYPE(i)];
	}
	mButtons.clear();
	mMenu_items.clear();
}

void Menu::show_tab(BUILDINGTYPE open_tab)
{
	this->mOpen_tab = open_tab;

	//enable all the items in the opened tab, disable all the items in the not opened tabs
	for (auto j = 0; j < BUILDINGTYPE::TYPES_TOTAL; j++) {
		if (BUILDINGTYPE(j) == open_tab) {
			for (auto i = 0; i < mMenu_items[open_tab]->size(); i++)
			{
				mMenu_items[open_tab]->at(i)->set_rendering_enabled(true);
				mMenu_items[open_tab]->at(i)->enable();
			}
		}
		else
		{
			for (auto i = 0; i < mMenu_items[BUILDINGTYPE(j)]->size(); i++)
			{
				mMenu_items[BUILDINGTYPE(j)]->at(i)->set_rendering_enabled(false);
				mMenu_items[BUILDINGTYPE(j)]->at(i)->disable();
			}
		}
	}
}

void Menu::on_button_press(int button_id)
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

	for (auto j = 0; j < BUILDINGTYPE::TYPES_TOTAL; j++) {
		for (auto i = 0; ; i++)
		{
			coords.x = 1300 + (i % number_of_items_per_row) * TILE_WIDTH;
			coords.y = 64 + (i / number_of_items_per_row) * TILE_HEIGHT;

			name_of_object.assign(gConfig_file->Value(types.at(j), std::to_string(i)));
			if (name_of_object == "end")
			{
				break;
			}
			auto new_item = new MenuItem(name_of_object, mLevel, coords);
			this->add_menu_item(new_item, BUILDINGTYPE(j));
		}
	}
}

void Menu::render()
{
	SDL_Color text_color = { 0, 0, 255 };

	//if there is a texture from last time, delete it
	if (mMenu_texture != nullptr)
	{
		mMenu_texture->free();
		delete mMenu_texture;
	}

	this->mMenu_texture = new LTexture();
	
	mMenu_texture->load_from_rendered_text("lives: " + std::to_string(mLevel->get_lives()) + 
		" \tgold: " + std::to_string(mLevel->get_resources()->get_resource(GOLD)) +
		" \twood: " + std::to_string(mLevel->get_resources()->get_resource(WOOD)) + 
		" \tstone: " + std::to_string(mLevel->get_resources()->get_resource(STONE)) +
		" \tiron: " + std::to_string(mLevel->get_resources()->get_resource(IRON)) + 
		" \tenergy: " + std::to_string(mLevel->get_resources()->get_resource(ENERGY)) +
		" \twater: " + std::to_string(mLevel->get_resources()->get_resource(WATER)) + 
		" \tfood: " + std::to_string(mLevel->get_resources()->get_resource(FOOD)), text_color);
	
	SDL_Rect dest;
	dest.x = 0;
	dest.y = 1050;
	dest.w = this->mMenu_texture->get_width();
	dest.h = this->mMenu_texture->get_height();

	gLayer_handler->render_to_layer(this->mMenu_texture, LAYERS::WINDOWS, nullptr, &dest);
}

void Menu::add_menu_item(MenuItem* menu_item, BUILDINGTYPE tab)
{
	this->mMenu_items[tab]->push_back(menu_item);
}
