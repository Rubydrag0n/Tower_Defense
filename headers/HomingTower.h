#pragma once
#include "Tower.h"

class HomingTower final :
	public Tower
{
public:
	HomingTower(const std::string& tower_name, SDL_Point coords, Level* level);

	void create_shot(Enemy* enemy) override;
};

