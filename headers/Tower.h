#pragma once
#include "Building.h"
class Tower : 
	public Building
{
public:
	Tower(std::string tower_name);
	~Tower();

	void placeTower(int x, int y);

private:
	std::string mTower_name;
};

