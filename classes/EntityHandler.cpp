#include "EntityHandler.h"
#include "Entity.h"

EntityHandler* gEntity_handler = nullptr;

EntityHandler::EntityHandler() : mEntities()
{

}

EntityHandler::~EntityHandler()
{
	mEntities.empty();
}

void EntityHandler::add_entity(Entity* u)
{
	this->mEntities.push_back(u);
}

void EntityHandler::del_entity(Entity* u)
{
	for (auto it = mEntities.begin(); it != mEntities.end(); it++)
	{
		if(*it == u)
		{
			mEntities.erase(it);
			break;
		}
	}
}

void EntityHandler::update()
{
	//can't use an iterator here because the particles will sometimes suicide and invalidate the iterator
	for (int i = this->mEntities.size() - 1; i >= 0; --i) {
		mEntities.at(i)->on_tick();
	}
}

std::vector<Enemy*> EntityHandler::get_enemies()
{
	auto v = std::vector<Enemy*>();	
	auto end = mEntities.end();
	for (auto it = this->mEntities.begin(); it != end; ++it)
	{
		if ((*it)->get_type() == ENTITYTYPE::ENEMY)
		{
			v.push_back(dynamic_cast<Enemy*>(*it));
		}
	}
	return v;
}

