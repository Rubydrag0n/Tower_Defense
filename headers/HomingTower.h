#pragma once
#include "Tower.h"


class HomingTower :
	public Tower
{
public:
	HomingTower(std::string tower_name, SDL_Point coords, Level* level);
	~HomingTower();

	void render_shot(Shot* shot) const override;
	Shot* create_shot(Enemy* enemy) override;

	//looks if the shot is at the enemy or the enemy is dead, if true shot is deleted and enemy gets dmg
	bool update_shot(Shot* shot, std::vector<Enemy*> all_enemies) override;
};

