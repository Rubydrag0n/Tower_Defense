#include <MouseHandler.h>
#include <SDL.h>
#include <vector>

MouseHandler* gMouse_handler = nullptr;

MouseHandler::MouseHandler() : mClickables{}
{
	
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
	for (auto it = mClickables.begin(); it != mClickables.end(); )
	{
		if (*it == c)
		{
			it = mClickables.erase(it);
			break;
		}
	}
}

void MouseHandler::update()
{
	int mouse_x, mouse_y;
	//get coordinates
	SDL_GetMouseState(&mouse_x, &mouse_y);

	auto end = this->mClickables.end();

	for (auto it = this->mClickables.begin(); it != end; ++it)
	{
		auto rect = (*it)->get_clickable_space();
		if (mouse_x > rect.x && mouse_x < rect.x + rect.w && 
			mouse_y > rect.y && mouse_y < rect.y + rect.h)
		{
			(*it)->on_mouse_over(mouse_x, mouse_y);
		}
	}
}

void MouseHandler::handle_event(SDL_Event *e)
{
	auto end = this->mClickables.end();
	auto x = e->button.x;
	auto y = e->button.y;
	int i = 0;
	//for (auto it = this->mClickables.begin(); it != end; ++it, i++)
	for(auto i = 0; i<mClickables.size(); i++)
	{
		Clickable* it = mClickables.at(i);
		auto rect = (it)->get_clickable_space();
		if (x > rect.x && x < rect.x + rect.w && y > rect.y && y < rect.y + rect.h)
		{
			switch (e->type)
			{
			case SDL_MOUSEBUTTONDOWN:
				/*printf("We got a Button-Down event.\n");
				printf("Current mouse position is: (%d, %d)\n", x, y);
				printf("Pressed button is: %hhu\n", e->button.button);
				*/

				if (e->button.type == 1025) {
					(it)->on_click(x, y);
				}
				else if (e->button.type == 2)
				{
					(it)->on_middle_click(x, y);
				}
				else if (e->button.type == 3)
				{
					(it)->on_right_click(x, y);
				}
				break;
			case SDL_MOUSEMOTION:
				/*
				printf("We got a Mouse-Move event.\n");
				printf("Current mouse position is: (%d, %d)\n", x, y);
				for (auto it = this->mClickables.begin(); it != end; ++it)
				{
					auto rect = (*it)->get_clickable_space();
					if (x > rect.x && x < rect.x + rect.w && y > rect.y && y < rect.y + rect.h)
					{
						(*it)->on_mouse_over(x, y);
					}
				} */
				break;
			default:
				//printf("Unhandled Event!\n");
				break;
			}
		}
	}
}
