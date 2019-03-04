#pragma once
#include "Level.h"
#include "MenuItem.h"
#include "Button.h"
#include "ButtonObject.h"
#include "Enums.h"

class MenuItem;

class Menu final
	: public Renderable, public ButtonObject
{
public:
	explicit Menu(Level *level);
	~Menu();

	void sort_items_into_menu();
	void show_tab(BUILDINGTYPE open_tab);
	void add_menu_item(MenuItem* menu_item, BUILDINGTYPE tab);

	void on_button_press(int button_id, Button* button) override;
	void render() override; //shows lives and resources

private:
	Level* mLevel;
	LTexture* mMenu_texture;

	std::map<BUILDINGTYPE, std::vector<MenuItem*>*> mMenu_items;
	std::map<BUILDINGTYPE, Button*> mButtons;

	BUILDINGTYPE mOpen_tab;
};
