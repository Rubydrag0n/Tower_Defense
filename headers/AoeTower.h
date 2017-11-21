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
	//checks if the tower can shoot at an enemy
	void update(std::vector<Enemy*> all_enemies);
	void shoot(std::vector<Enemy*> all_enemies);


private:
	double mExplosive_radius;
	std::vector<AoeShot*> mShots;
};

