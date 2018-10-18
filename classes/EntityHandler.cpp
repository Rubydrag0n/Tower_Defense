#include "EntityHandler.h"
#include "Entity.h"

EntityHandler* gEntity_handler = nullptr;

EntityHandler::EntityHandler() : mEntities(), mEntities_by_type(), mEntityvector_dirty_bit()
{
	//initialize mEntities_by_type and set all dirtybits to true
	for (int i = 0; i < int(ENTITYTYPE::ENTITYTYPES_TOTAL); i++) {
		this->mEntities_by_type[ENTITYTYPE(i)] = new std::vector<Entity*>();
		this->mEntityvector_dirty_bit[ENTITYTYPE(i)] = true;
	}
}

EntityHandler::~EntityHandler()
{
	mEntities.empty();
}

void EntityHandler::add_entity(Entity* u)
{
	this->invalidate_entity_vectors();
	this->mEntities.push_back(u);
}

void EntityHandler::del_entity(Entity* u)
{
	this->invalidate_entity_vectors();
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

const std::vector<Entity*>* EntityHandler::get_entities_of_type(ENTITYTYPE type)
{
	if (!this->mEntityvector_dirty_bit[type]) {
		return this->mEntities_by_type[type];
	}

	//if dirtybit was set, delete old vector here
	delete this->mEntities_by_type[type];

	auto v = new std::vector<Entity*>();

	auto end = mEntities.end();
	for (auto it = this->mEntities.begin(); it != end; ++it)
	{
		if ((*it)->get_type() == type)
		{
			v->push_back(*it);
		}
	}

	//save the updated vector and set dirtybit back to false
	this->mEntities_by_type[type] = v;
	this->mEntityvector_dirty_bit[type] = false;

	return v;
}

void EntityHandler::invalidate_entity_vectors()
{
	for (int i = 0; i < int(ENTITYTYPE::ENTITYTYPES_TOTAL); i++) {
		this->mEntityvector_dirty_bit[ENTITYTYPE(i)] = true;
	}
}

