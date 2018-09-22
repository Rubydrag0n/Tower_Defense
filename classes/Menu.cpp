#include "Menu.h"
#include <SDL.h>
#include "LTexture.h"
#include "LayerHandler.h"
#include "ConfigFile.h"
#include <functional>
#include "SDL_setup.h"

MENUTAB gOpen_tab;

void show_tower()
{
	gOpen_tab = TOWER_TAB;
}


void show_industrial_buildings()
{
	gOpen_tab = INDUSTRIAL_BUILDING_TAB;
}


Menu::Menu(Level *level)
{
	mLevel = level;
	mMenu_texture = nullptr;
	gOpen_tab = TOWER_TAB;
	SDL_Rect dim;
	dim.x = 1300;
	dim.y = 0;
	dim.w = 100;
	dim.h = 26;
	mTab_tower = new Button("testbutton", dim, &show_tower);
	dim.x = 1400;
	mTab_industrial_buildings = new Button("testbutton", dim, &show_industrial_buildings);

	this->sort_items_into_menu();

}



Menu::~Menu()
{
}

void Menu::sort_items_into_menu()
{
	SDL_Point coords;
	std::string name_of_object;
	auto all_tower_sorted = false;
	int const number_of_items_in_one_row = 5;
	auto all_industrial_buildings_sorted = false;
	for (auto i = 0; !(all_tower_sorted && all_industrial_buildings_sorted); i++)
	{
		coords.x = 1300 + (i % number_of_items_in_one_row) * TILE_WIDTH;
		coords.y = 64 + (i / number_of_items_in_one_row) * TILE_HEIGHT;

		if(!all_tower_sorted)
		{
			name_of_object.assign(gConfig_file->Value("tower", std::to_string(i)));
			if (name_of_object == "end")
			{
				all_tower_sorted = true;
			}
			if(!all_tower_sorted)
			{
				auto new_item = new MenuItem(name_of_object, mLevel, coords);
				this->add_menu_item_tower(new_item);
			}
		}

		if(!all_industrial_buildings_sorted)
		{
			name_of_object.assign(gConfig_file->Value("industrialbuildings", std::to_string(i)));
			if (name_of_object == "end")
			{
				all_industrial_buildings_sorted = true;
			}
			if(!all_industrial_buildings_sorted)
			{
				auto new_item = new MenuItem(name_of_object, mLevel, coords);
				this->add_menu_item_industrialbuilding(new_item);
			}
		}
	}

}


void Menu::render(SDL_Point mouse_position)
{
	SDL_Color text_color = { 0, 0, 255 };

	//if there is a texture from last time, delete it
	if (mMenu_texture != nullptr)
	{
		mMenu_texture->free();
		delete mMenu_texture;
	}

	this->mMenu_texture = new LTexture();
	
	mMenu_texture->load_from_rendered_text("lives: " + std::to_string(mLevel->get_lives()) + " \tgold: " + std::to_string(mLevel->get_ressources()->get_gold()) +
		" \twood: " + std::to_string(mLevel->get_ressources()->get_wood()) + " \tstone: " + std::to_string(mLevel->get_ressources()->get_stone()) +
		" \tiron: " + std::to_string(mLevel->get_ressources()->get_iron()) + " \tenergy: " + std::to_string(mLevel->get_ressources()->get_energy()) +
		" \twater: " + std::to_string(mLevel->get_ressources()->get_water()) + " \tfood: " + std::to_string(mLevel->get_ressources()->get_food()), text_color);
	
	SDL_Rect dest;
	dest.x = 0;
	dest.y = 1050;
	dest.w = this->mMenu_texture->get_width();
	dest.h = this->mMenu_texture->get_height();

	gLayer_handler->render_to_layer(this->mMenu_texture, LAYERS::OVERLAY, nullptr, &dest);

	if(gOpen_tab == TOWER_TAB)
	{
		for (auto i = 0; i < mMenu_items_tower.size(); i++)
		{
			mMenu_items_tower.at(i)->render(mouse_position);
		}
	}
	else
	{
		for (auto i = 0; i < mMenu_items_tower.size(); i++)
		{
			mMenu_items_tower.at(i)->delete_clickable_space();
		}
	}
	if (gOpen_tab == INDUSTRIAL_BUILDING_TAB)
	{
		for (auto i = 0; i < mMenu_items_industrial_buildings.size(); i++)
		{
			mMenu_items_industrial_buildings.at(i)->render(mouse_position);
		}
	}
	else
	{
		for (auto i = 0; i < mMenu_items_industrial_buildings.size(); i++)
		{
			mMenu_items_industrial_buildings.at(i)->delete_clickable_space();
		}
	}

}

void Menu::add_menu_item_tower(MenuItem* menu_item)
{
	mMenu_items_tower.push_back(menu_item);
}


void Menu::add_menu_item_industrialbuilding(MenuItem* menu_item)
{
	mMenu_items_industrial_buildings.push_back(menu_item);
}

