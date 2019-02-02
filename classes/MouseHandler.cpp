#include <SDL.h>
#include <vector>

#include "MouseHandler.h"
#include "SDL_setup.h"

MouseHandler* gMouse_handler = nullptr;

MouseHandler::MouseHandler() : mCurrent_mouse_position{0, 0}
{
	mItem_on_mouse = nullptr;
}

MouseHandler::~MouseHandler()
{
	mClickables.clear();
}

void MouseHandler::add_clickable(Clickable* c)
{
	this->mClickables.push_back(c);
}

void MouseHandler::del_clickable(Clickable* c)
{
	for (auto it = mClickables.begin(); it != mClickables.end(); ++it)
	{
		if (*it == c)
		{
			it = mClickables.erase(it);
			break;
		}
	}
}

//handles the mouse_over events, since they are not (always) an SDL_Event
void MouseHandler::update()
{
	int mouse_x, mouse_y;
	//get coordinates
	SDL_GetMouseState(&mouse_x, &mouse_y);
	//set the mouse position for this tick
	this->set_mouse_position(mouse_x, mouse_y);
	this->get_mouse_position(&mouse_x, &mouse_y);

	const auto end = this->mClickables.end();

	for (auto it = this->mClickables.begin(); it != end; ++it)
	{
		const auto rect = (*it)->get_clickable_space();
		if (mouse_x > rect.x && mouse_x < rect.x + rect.w && 
			mouse_y > rect.y && mouse_y < rect.y + rect.h && 
			(*it)->is_enabled())
		{
			(*it)->on_mouse_over(mouse_x, mouse_y);
			(*it)->set_state(L_CLICKABLE_STATE::MOUSE_OVER);
		} 
		else 
		{
			(*it)->set_state(L_CLICKABLE_STATE::MOUSE_OUT);
		}
	}
}

void MouseHandler::handle_event(SDL_Event *e)
{
	auto end = this->mClickables.end();
	const auto x = e->button.x;
	const auto y = e->button.y;
	for (auto it : mClickables)
	{
		if (e->button.button == 1 && e->button.state == SDL_PRESSED)
		{
			it->set_clicked(false);
		}
		const auto rect = it->get_clickable_space();
		if (x > rect.x && x < rect.x + rect.w && y > rect.y && y < rect.y + rect.h
			&& it->is_enabled())
		{
			switch (e->type)
			{
			case SDL_MOUSEBUTTONDOWN:
				if (e->button.button == 1) {
					it->set_state(L_CLICKABLE_STATE::MOUSE_DOWN_LEFT);
					it->on_click(x, y);
				}
				else if (e->button.button == 2)
				{
					it->set_state(L_CLICKABLE_STATE::MOUSE_DOWN_MIDDLE);
					it->on_middle_click(x, y);
				}
				else if (e->button.button == 3)
				{
					it->set_state(L_CLICKABLE_STATE::MOUSE_DOWN_RIGHT);
					it->on_right_click(x, y);
				}
				break;
			case SDL_MOUSEMOTION:
				it->set_state(L_CLICKABLE_STATE::MOUSE_OVER);
				it->on_mouse_over(x, y);
				break;
			default:
				break;
			}
		}
		else 
		{
			it->set_state(L_CLICKABLE_STATE::MOUSE_OUT);
		}
	}
}

MouseItem* MouseHandler::get_item_on_mouse() const
{
	return mItem_on_mouse;
}

void MouseHandler::set_item_on_mouse(MouseItem* item)
{
	delete mItem_on_mouse;
	mItem_on_mouse = item;
}

void MouseHandler::set_mouse_position(const int x, const int y)
{
	//convert to 1920x1080 from the actual resolution
	this->mCurrent_mouse_position.x = static_cast<int>(double(x) / *gActual_screen_width * LOGICAL_SCREEN_WIDTH);
	this->mCurrent_mouse_position.y = static_cast<int>(double(y) / *gActual_screen_height * LOGICAL_SCREEN_HEIGHT);
}

void MouseHandler::get_mouse_position(int *x, int *y) const
{
	*x = this->mCurrent_mouse_position.x;
	*y = this->mCurrent_mouse_position.y;
}
