#include "Menu.h"
#include "SDL_setup.h"
#include <SDL.h>
#include "LTexture.h"



Menu::Menu(Level *level)
{
	mLevel = level;
}


Menu::~Menu()
{
}


void Menu::render() const
{
	SDL_Color text_color = { 0, 0, 255 };

	//shows lives and ressources
	auto *menu_resources = new LTexture();
	
	menu_resources->loadFromRenderedText("lives: " + std::to_string(mLevel->get_lives()) + "   gold: " + std::to_string(mLevel->get_ressources()->get_gold()) +
		"  wood: " + std::to_string(mLevel->get_ressources()->get_wood()) + "  stone: " + std::to_string(mLevel->get_ressources()->get_stone()) +
		"  iron: " + std::to_string(mLevel->get_ressources()->get_iron()) + "  energy: " + std::to_string(mLevel->get_ressources()->get_energy()) +
		"  water: " + std::to_string(mLevel->get_ressources()->get_water()) + "  food: " + std::to_string(mLevel->get_ressources()->get_food()), text_color);
	menu_resources->render(0, 1050, nullptr, 0, nullptr, SDL_FLIP_NONE);

	menu_resources->free();

	delete menu_resources;
}

