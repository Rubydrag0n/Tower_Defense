#pragma once
#include "Entity.h"
#include "Enemy.h"
#include <vector>
#include <map>

class EntityHandler
{
public:
	EntityHandler();
	~EntityHandler();

	void add_entity(Entity* u);
	void del_entity(Entity* u);

	void update();

	const std::vector<Entity*>* get_entities_of_type(ENTITYTYPE type);

private:
	void invalidate_entity_vectors();

	std::vector<Entity*> mEntities;

	std::map<ENTITYTYPE, std::vector<Entity*>*> mEntities_by_type;

	std::map<ENTITYTYPE, bool> mEntityvector_dirty_bit;
};

extern EntityHandler* gEntity_handler;
