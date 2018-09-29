#include "HomingTower.h"
#include "HomingShot.h"

HomingTower::HomingTower(std::string tower_name, SDL_Point coords, Level* level) : Tower(tower_name, coords, level)
{
}

HomingTower::~HomingTower()
{
}

void HomingTower::create_shot(Enemy* enemy) 
{
	new HomingShot(this, enemy);
}
