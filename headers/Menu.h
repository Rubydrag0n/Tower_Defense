#pragma once
#include "Level.h"
#include "MenuItem.h"
#include "CoordinatesInDouble.h"
#include "Button.h"
#include "ButtonObject.h"

enum MENUTAB
{
	TOWER_TAB = 0,
	INDUSTRIAL_BUILDING_TAB = 1,
	TABS_TOTAL = 2
};

enum BUTTONIDS
{
	TOWERS_BUTTON,
	INDUSTRIAL_BUILDINGS_BUTTON,
	NUMBER_OF_BUTTONS
};

class Menu 
	: public Renderable, public ButtonObject
{
public:
	Menu(Level *level);
	~Menu();

	void render() override; //shows lives and ressources
	void add_menu_item_tower(MenuItem* menu_item);
	void Menu::add_menu_item_industrialbuilding(MenuItem* menu_item);
	void sort_items_into_menu();
	void show_towers();
	void show_industrial_buildings();
	void on_button_press(int button_id) override;

private:
	Level* mLevel;
	LTexture* mMenu_texture;
	std::vector<MenuItem*> mMenu_items_tower;
	std::vector<MenuItem*> mMenu_items_industrial_buildings;
	Button* mTab_tower_button;
	Button* mTab_industrial_buildings_button;

	MENUTAB mOpen_tab;
};
