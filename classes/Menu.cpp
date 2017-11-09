#include "Menu.h"
#include <SDL_ttf.h>
#include "SDL_setup.h"
#include "LTexture.h"



Menu::Menu(Level *level)
{
	mLevel = level;
}


Menu::~Menu()
{
}


void Menu::render()
{
	SDL_Color text_color = { 0, 0, 255 };
	
	//shows lives and ressources
	LTexture menu_ressources;
	menu_ressources.loadFromRenderedText("lives: " + std::to_string(mLevel->get_lives()) + "   gold: " + std::to_string(mLevel->get_ressources().get_gold()) +
		"  wood: " + std::to_string(mLevel->get_ressources().get_wood()) + "  stone: " + std::to_string(mLevel->get_ressources().get_stone()) +
		"  iron: " + std::to_string(mLevel->get_ressources().get_iron()) + "  energy: " + std::to_string(mLevel->get_ressources().get_energy()) +
		"  water: " + std::to_string(mLevel->get_ressources().get_water()) + "  food: " + std::to_string(mLevel->get_ressources().get_food()), text_color);
	menu_ressources.render(0, 1050, nullptr, 0, nullptr, SDL_FLIP_NONE);
}

