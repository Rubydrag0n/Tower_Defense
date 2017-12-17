#include "HomingTower.h"
#include "HomingShot.h"


HomingTower::HomingTower(std::string tower_name, SDL_Point coords, Level* level) : Tower(tower_name, coords, level)
{
}


HomingTower::~HomingTower()
{
}

void HomingTower::render_shot(Shot* shot) const
{
	//shot in here will be a HomingShot
	auto homing_shot = static_cast<HomingShot*>(shot);
	homing_shot->render(homing_shot->get_enemy_to_shoot()->get_position());
}

Shot* HomingTower::create_shot(Enemy* enemy) 
{
	return new HomingShot(this, enemy);
}

bool HomingTower::update_shot(Shot* shot, std::vector<Enemy*> all_enemies)
{
	auto homing_shot = static_cast<HomingShot*>(shot);
	if (homing_shot->get_enemy_to_shoot()->is_dead() || homing_shot->follow())
	{
		if (homing_shot->follow())
		{
			homing_shot->get_enemy_to_shoot()->take_damage(&mDamage);
		}
		delete homing_shot;
		return true;
	}
	return false;
}



