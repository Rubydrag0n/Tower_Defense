#include "MenuItem.h"
#include "ConfigFile.h"
#include "SDL_setup.h"
#include "LayerHandler.h"

MenuItem::MenuItem(std::string name_of_object, Level *level) 
{
	mSprite = gTextures->get_texture(gConfig_file->Value(name_of_object + "/sprite", "path"));
	mCoords.x = 1400;
	mCoords.y = 100;
	mLevel = level;
	SDL_Rect clickable;
	clickable.x = mCoords.x - mSprite->get_width() / 2;
	clickable.y = mCoords.y - mSprite->get_height() / 2;
	clickable.w = mSprite->get_width();
	clickable.h = mSprite->get_height();
	this->set_clickable_space(clickable);
	mClickstate = CLICKSTATE::UNCLICKED;
}

MenuItem::~MenuItem()
{
	
}

void MenuItem::render(SDL_Point mouse_position)
{
	SDL_Rect dest;
	dest.x = this->get_coords().x - this->get_sprite()->get_width() / 2;
	dest.y = this->get_coords().y - this->get_sprite()->get_height() / 2;
	dest.w = this->get_sprite()->get_width();
	dest.h = this->get_sprite()->get_height();
	gLayer_handler->render_to_layer(this->get_sprite(), LAYERS::BACKGROUND, nullptr, &dest);
	if (this->getClickstate() == CLICKSTATE::LEFTCLICKED)
	{
		dest.x = mouse_position.x - this->get_sprite()->get_width() / 2;
		dest.y = mouse_position.y - this->get_sprite()->get_height() / 2;
		gLayer_handler->render_to_layer(this->get_sprite(), LAYERS::OVERLAY, nullptr, &dest);
	}
}


void MenuItem::on_click(int mouse_x, int mouse_y)
{
	mClickstate = CLICKSTATE::LEFTCLICKED;
}

void MenuItem::on_middle_click(int mouse_x, int mouse_y)
{

}

void MenuItem::on_mouse_over(int mouse_x, int mouse_y)
{

}

void MenuItem::on_right_click(int mouse_x, int mouse_y)
{

}


LTexture* MenuItem::get_sprite()
{
	return this->mSprite;
}

SDL_Point MenuItem::get_coords()
{
	return this->mCoords;
}

CLICKSTATE MenuItem::getClickstate()
{
	return this->mClickstate;
}










