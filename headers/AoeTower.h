#pragma once
#include "Tower.h"
#include "AoeShot.h"

class AoeTower : 
	public Tower
{
public:
	AoeTower(std::string tower_name, SDL_Point coords, Level* level);
	~AoeTower();

	void render_shot(Shot* shot) const override;
	Shot* create_shot(Enemy* enemy) override;

	// looks if the shot is at his location, if true everything in the radius takes dmg and the shot is deleted
	bool update_shot(Shot* shot) override;


private:
	double mExplosive_radius;
};

