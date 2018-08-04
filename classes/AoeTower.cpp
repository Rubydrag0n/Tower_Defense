#include "AoeTower.h"
#include "AoeShot.h"
#include "ConfigFile.h"


AoeTower::AoeTower(std::string tower_name, SDL_Point coords, Level* level) : Tower(tower_name, coords, level)
{
	mExplosive_radius = gConfig_file->Value(mTower_name + "/stats", "explosiveradius");
}


AoeTower::~AoeTower()
{
}

void AoeTower::render_shot(Shot* shot) const
{
	//shot here will always be AoeShot
	auto s = static_cast<AoeShot*>(shot);
	s->render(s->get_location_to_shoot());
}

Shot* AoeTower::create_shot(Enemy* enemy)
{
	return new AoeShot(this, enemy->get_position());
}

bool AoeTower::update_shot(Shot* shot, std::vector<Enemy*> all_enemies)
{
	auto aoe_shot = static_cast<AoeShot*>(shot);
	if (aoe_shot->follow())
	{
		while (!all_enemies.empty())
		{
			if (enemy_in_range(all_enemies.at(0), mExplosive_radius, aoe_shot->get_coords()))
			{
				all_enemies.at(0)->take_damage(&mDamage);
			}
			all_enemies.erase(all_enemies.begin());
		}
		delete aoe_shot;
		return true;
	}
	return false;
}

