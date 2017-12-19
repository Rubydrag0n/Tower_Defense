#pragma once
#include "Clickable.h"

class Updateable
{
public:
	Updateable();
	virtual ~Updateable();

	virtual void update() = 0;

private:
};
