#pragma once
#include "Tower.h"
#include "AoeShot.h"

class AoeTower : 
	public Tower
{
public:
	AoeTower(std::string tower_name, SDL_Point coords, Level* level);
	~AoeTower();

	void render();
	void update(std::vector<Enemy*> all_enemies);
	void shoot(std::vector<Enemy*> all_enemies);


private:
	double mExplosiveRadius;
	std::vector<AoeShot*> mShots;
};

