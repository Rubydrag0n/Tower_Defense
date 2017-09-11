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
	LTexture some_text;
	SDL_Color text_color = { 255, 255, 255 };
	some_text.loadFromRenderedText("something", text_color);

	some_text.render(0, 0, nullptr, 0, nullptr, SDL_FLIP_NONE);
}

