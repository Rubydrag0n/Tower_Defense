#pragma once
#include "Level.h"
#include "MenuItem.h"
#include "CoordinatesInDouble.h"
#include "Button.h"
#include "ButtonObject.h"

enum BUILDINGTYPE
{
	TOWER,
	INDUSTRIAL_BUILDING,
	LOGISTICS_BUILDING,
	TYPES_TOTAL
};

class Menu 
	: public Renderable, public ButtonObject
{
public:
	Menu(Level *level);
	~Menu();

	void sort_items_into_menu();
	void show_tab(BUILDINGTYPE open_tab);
	void add_menu_item(MenuItem* menu_item, BUILDINGTYPE tab);

	void on_button_press(int button_id) override;
	void render() override; //shows lives and ressources

private:
	Level* mLevel;
	LTexture* mMenu_texture;

	std::map<BUILDINGTYPE, std::vector<MenuItem*>*> mMenu_items;
	std::map<BUILDINGTYPE, Button*> mButtons;

	BUILDINGTYPE mOpen_tab;
};
