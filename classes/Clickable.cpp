#include "MouseHandler.h"
#include "Clickable.h""
#include <SDL.h>

Clickable::Clickable() : mClickable_space{}
{
	//add this object to the list of clickable objects
	gMouse_handler->add_clickable(this);
	this->mState = LClickableState::MOUSE_OUT;
	mClicked = false;
}

Clickable::~Clickable()
{
	//delete this object from the list of clickable objects
	gMouse_handler->del_clickable(this);
}

SDL_Rect Clickable::get_clickable_space() const
{
	return this->mClickable_space;
}

void Clickable::set_clickable_space(SDL_Rect clickable_space)
{
	this->mClickable_space = clickable_space;
}

LClickableState Clickable::get_state() const
{
	return this->mState;
}

void Clickable::set_state(LClickableState state)
{
	this->mState = state;
}

bool Clickable::get_clicked()
{
	return mClicked;
}

void Clickable::set_clicked(bool value)
{
	mClicked = value;
}

