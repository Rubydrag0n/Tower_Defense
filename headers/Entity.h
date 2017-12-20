#pragma once

enum class ENTITYTYPE
{
	UNDEFINED,
	TOWER,
	ENEMY
};

class Entity
{
public:
	Entity();
	virtual ~Entity();

	virtual void on_tick();
	virtual void render();

	virtual ENTITYTYPE get_type();
};
