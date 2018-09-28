#pragma once
#include "Renderable.h"

enum class ENTITYTYPE
{
	UNDEFINED,
	TOWER,
	ENEMY,
	SHOT,
	PARTICLE,
	COUNT
};

class Entity :
	public Renderable
{
public:
	Entity();
	virtual ~Entity();

	virtual void on_tick() = 0;

	virtual ENTITYTYPE get_type();
};
