#include "MouseHandler.h"
#include "Clickable.h"
#include <SDL.h>

Clickable::Clickable() : mClickable_space{}, mClicked{false}, mEnabled{true}, mDepth{0}
{
	//add this object to the list of clickable objects
	gMouse_handler->add_clickable(this);
	this->mState = L_CLICKABLE_STATE::MOUSE_OUT;
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

void Clickable::set_clickable_space(const SDL_Rect clickable_space)
{
	this->mClickable_space = clickable_space;
}

L_CLICKABLE_STATE Clickable::get_state() const
{
	return this->mState;
}

void Clickable::set_state(const L_CLICKABLE_STATE state)
{
	this->mState = state;
}

bool Clickable::is_clicked() const
{
	return mClicked;
}

void Clickable::set_clicked(const bool value)
{
	mClicked = value;
}

void Clickable::set_depth(const unsigned depth)
{
	mDepth = depth;
}

unsigned Clickable::get_depth() const
{
	return mDepth;
}

void Clickable::enable()
{
	mEnabled = true;
}

void Clickable::disable()
{
	mEnabled = false;
}

bool Clickable::is_enabled() const
{
	return this->mEnabled;
}
