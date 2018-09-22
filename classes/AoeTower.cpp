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

bool AoeTower::update_shot(Shot* shot)
{
	auto aoe_shot = static_cast<AoeShot*>(shot);
	if (aoe_shot->follow())
	{
		auto all_enemies = gEntity_handler->get_entities_of_type(ENTITYTYPE::ENEMY);
		auto end = all_enemies->end();
		for (auto it = all_enemies->begin(); it != end; ++it)
		{
			if (enemy_in_range(dynamic_cast<Enemy*>(*it), mExplosive_radius, aoe_shot->get_coords()))
			{
				dynamic_cast<Enemy*>(*it)->take_damage(&mDamage);
			}
		}
		delete aoe_shot;
		return true;
	}
	return false;
}

