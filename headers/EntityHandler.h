#pragma once
#include "Entity.h"
#include "Enemy.h"

class EntityHandler
{
public:
	EntityHandler();
	~EntityHandler();

	void add_entity(Entity* u);
	void del_entity(Entity* u);

	void update();

	std::vector<Enemy*> get_enemies();

private:
	std::vector<Entity*> mEntities;
};

extern EntityHandler* gEntity_handler;
