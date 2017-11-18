#pragma once
#include "Tower.h"
#include "HomingShot.h"

class HomingTower :
	public Tower
{
public:
	HomingTower(std::string tower_name, SDL_Point coords, Level* level);
	~HomingTower();

	void render();
	void update(std::vector<Enemy*> all_enemies);
	void shoot();

private:
	std::vector<HomingShot*> mShots;
};

