#pragma once
#include "Tower.h"

class AoeTower final : 
	public Tower
{
public:
	AoeTower(const std::string& tower_name, SDL_Point coords, Level* level);

	void create_shot(Enemy* enemy) override;

private:
	double mExplosive_radius;
};
