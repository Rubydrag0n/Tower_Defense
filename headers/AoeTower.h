#pragma once
#include "Tower.h"
#include "AoeShot.h"

class AoeTower : 
	public Tower
{
public:
	AoeTower(std::string tower_name, SDL_Point coords, Level* level);
	~AoeTower();

	void create_shot(Enemy* enemy) override;

private:
	double mExplosive_radius;
};
