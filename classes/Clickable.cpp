#include <Clickable.h>
#include <SDL.h>

Clickable::Clickable() : mClickable_space{}
{
	
}

Clickable::~Clickable()
{
	
}

SDL_Rect Clickable::get_clickable_space() const
{
	return this->mClickable_space;
}

void Clickable::set_clickable_space(SDL_Rect clickable_space)
{
	this->mClickable_space = clickable_space;
}
