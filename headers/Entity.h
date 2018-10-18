#pragma once
#include "Renderable.h"
#include "Enums.h"

class Entity :
	public Renderable
{
public:
	Entity();
	virtual ~Entity();

	virtual void on_tick() = 0;

	virtual ENTITYTYPE get_type();
};
