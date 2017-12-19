#include "UpdateHandlerh.h"

UpdateHandler* gUpdate_handler = nullptr;

UpdateHandler::UpdateHandler()
{
	
}

UpdateHandler::~UpdateHandler()
{
	
}


void UpdateHandler::add_updateable(Updateable* u)
{
	this->mUpdateables.push_back(u);
}

void UpdateHandler::del_updateable(Updateable* u)
{
	for (auto it = mUpdateables.begin(); it != mUpdateables.end(); it++)
	{
		if(*it == u)
		{
			mUpdateables.erase(it);
			break;
		}
	}
}

void UpdateHandler::update()
{
	for(auto it = this->mUpdateables.begin(); it != mUpdateables.end(); it++)
	{
		(*it)->update();
	}
}
