#include "AoeTower.h"
#include "AoeShot.h"
#include "ConfigFile.h"
#include "EntityHandler.h"

AoeTower::AoeTower(std::string tower_name, SDL_Point coords, Level* level) : Tower(tower_name, coords, level)
{
	mExplosive_radius = gConfig_file->Value(mTower_name + "/stats", "explosiveradius");
}

AoeTower::~AoeTower()
{
}

void AoeTower::create_shot(Enemy* enemy)
{
	new AoeShot(this, enemy->get_position(), mExplosive_radius);
}
