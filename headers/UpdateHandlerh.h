#pragma once
#include "MouseHandler.h"
#include "Updateable.h"

class UpdateHandler
{
public:
	UpdateHandler();
	~UpdateHandler();

	void add_updateable(Updateable* u);
	void del_updateable(Updateable* u);

	void update();

private:
	std::vector<Updateable*> mUpdateables;
};

extern UpdateHandler* gUpdate_handler;
