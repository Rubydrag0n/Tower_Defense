#include "Menu.h"
#include "SDL_setup.h"
#include <SDL.h>
#include "LTexture.h"
#include "LayerHandler.h"

Menu::Menu(Level *level)
{
	mLevel = level;
	mMenu_texture = nullptr;

}

Menu::~Menu()
{
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
	
	mMenu_texture->load_from_rendered_text("lives: " + std::to_string(mLevel->get_lives()) + "   gold: " + std::to_string(mLevel->get_ressources()->get_gold()) +
		"  wood: " + std::to_string(mLevel->get_ressources()->get_wood()) + "  stone: " + std::to_string(mLevel->get_ressources()->get_stone()) +
		"  iron: " + std::to_string(mLevel->get_ressources()->get_iron()) + "  energy: " + std::to_string(mLevel->get_ressources()->get_energy()) +
		"  water: " + std::to_string(mLevel->get_ressources()->get_water()) + "  food: " + std::to_string(mLevel->get_ressources()->get_food()), text_color);
	
	SDL_Rect dest;
	dest.x = 0;
	dest.y = 1050;
	dest.w = this->mMenu_texture->get_width();
	dest.h = this->mMenu_texture->get_height();

	gLayer_handler->render_to_layer(this->mMenu_texture, LAYERS::OVERLAY, nullptr, &dest);

	for(auto i = 0; i<mMenu_items.size(); i++)
	{
		LTexture t;
		dest.x = mMenu_items.at(i)->get_coords().x;
		dest.y = mMenu_items.at(i)->get_coords().y;
		dest.w = mMenu_items.at(i)->get_sprite()->get_width();
		dest.h = mMenu_items.at(i)->get_sprite()->get_height();
		gLayer_handler->render_to_layer(mMenu_items.at(i)->get_sprite(), LAYERS::BACKGROUND, nullptr, &dest);
	}
}

void Menu::add_menu_item(MenuItem* menu_item)
{
	mMenu_items.push_back(menu_item);
}


