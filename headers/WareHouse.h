#pragma once
#include "Building.h"

class Warehouse 
	: public Building
{
public:
	Warehouse(std::string logistics_building_name, SDL_Point coords, Level *level);
	~Warehouse();

	void on_tick() override;
	void render() override;
};