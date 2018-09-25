#pragma once
#include "Level.h"
#include "MenuItem.h"
#include "CoordinatesInDouble.h"
#include "Button.h"


enum MENUTAB
{
	TOWER_TAB = 0,
	INDUSTRIAL_BUILDING_TAB = 1,
	TABS_TOTAL = 2
};



class Menu : public Renderable
{
public:
	Menu(Level *level);
	~Menu();

	void render() override; //shows lives and ressources
	void add_menu_item_tower(MenuItem* menu_item);
	void Menu::add_menu_item_industrialbuilding(MenuItem* menu_item);
	void sort_items_into_menu();

private:
	Level* mLevel;
	LTexture* mMenu_texture;
	std::vector<MenuItem*> mMenu_items_tower;
	std::vector<MenuItem*> mMenu_items_industrial_buildings;
	Button* mTab_tower;
	Button* mTab_industrial_buildings;

};
extern MENUTAB gOpen_tab;