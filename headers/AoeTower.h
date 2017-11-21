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
	bool update_shot(Shot* shot, std::vector<Enemy*> all_enemies) override;


private:
	double mExplosive_radius;
};

