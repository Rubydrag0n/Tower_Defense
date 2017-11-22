#pragma once
#include "Tower.h"


class HomingTower :
	public Tower
{
public:
	HomingTower(std::string tower_name, SDL_Point coords, Level* level);
	~HomingTower();

	void render_shot(Shot* shot) const override;
	bool update_shot(Shot* shot, std::vector<Enemy*> all_enemies) override;
	Shot* create_shot(Enemy* enemy) override;
};

