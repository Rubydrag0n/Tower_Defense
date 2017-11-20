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
	for (auto it = mClickables.begin(); it != mClickables.end(); ++it)
	{
		if (*it == c)
		{
			mClickables.erase(it);
			break;
		}
	}
}

void MouseHandler::handle_event(SDL_Event *e)
{
	auto end = this->mClickables.end();
	auto x = e->button.x;
	auto y = e->button.y;

	switch (e->type)
	{
	case SDL_MOUSEBUTTONDOWN:
		printf("We got a Button-Down event.\n");
		printf("Current mouse position is: (%d, %d)\n", x, y);
		printf("Pressed button is: %hhu\n", e->button.button);
		
		for (auto it = this->mClickables.begin(); it != end; ++it)
		{
			auto rect = (*it)->get_clickable_space();
			if (x > rect.x && x < rect.x + rect.w && y > rect.y && y < rect.y + rect.h)
			{
				(*it)->on_click();
			}
		}
		break;
	default:
		//printf("Unhandled Event!\n");
		break;
	}
}
