#include "AoeTower.h"
#include "AoeShot.h"
#include "ConfigFile.h"
#include "EntityHandler.h"

AoeTower::AoeTower(const std::string& tower_name, const SDL_Point coords, Level* level) : Tower(tower_name, coords, level)
{
	mExplosive_radius = gConfig_file->value(mTower_name + "/stats", "explosiveradius");
}

void AoeTower::create_shot(Enemy* enemy)
{
	new AoeShot(this, enemy->get_position(), mExplosive_radius);
}
