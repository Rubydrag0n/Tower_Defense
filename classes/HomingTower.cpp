#include "HomingTower.h"
#include "HomingShot.h"

HomingTower::HomingTower(const std::string& tower_name, const SDL_Point coords, Level* level) : Tower(tower_name, coords, level)
{
}

void HomingTower::create_shot(Enemy* enemy) 
{
	//These objects take care of themselves
	// ReSharper disable once CppNonReclaimedResourceAcquisition
	new HomingShot(this, enemy);
}
