#include "Updateable.h"
#include "UpdateHandlerh.h"


Updateable::Updateable()
{
	gUpdate_handler->add_updateable(this);
}

Updateable::~Updateable()
{
	gUpdate_handler->del_updateable(this);
}



