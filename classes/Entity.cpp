#include "Entity.h"
#include "EntityHandler.h"

Entity::Entity()
{
	gEntity_handler->add_entity(this);
}

Entity::~Entity()
{
	gEntity_handler->del_entity(this);
}

ENTITYTYPE Entity::get_type()
{
	return ENTITYTYPE::UNDEFINED;
}