#pragma once
#include "Tower.h"

class HomingTower :
	public Tower
{
public:
	HomingTower(std::string tower_name, SDL_Point coords, Level* level);
	~HomingTower();

	void create_shot(Enemy* enemy) override;
};

