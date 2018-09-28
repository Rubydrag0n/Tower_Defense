#include <SDL.h>

#include "EntityHandler.h"
#include "AoeShot.h"
#include "Particles.h"
#include "Tower.h"

AoeShot::AoeShot(Tower* tower, SDL_Point location_to_shoot, double explosion_radius) 
	: Shot(tower), mExplosive_radius{explosion_radius}
{
	mLocation_to_shoot = location_to_shoot;
	//target is set once the shot exists, it wont change
	this->set_target(location_to_shoot);
}

AoeShot::~AoeShot()
{
}

void AoeShot::on_tick()
{
	if (this->follow())
	{
		auto all_enemies = gEntity_handler->get_entities_of_type(ENTITYTYPE::ENEMY);
		auto end = all_enemies->end();
		for (auto it = all_enemies->begin(); it != end; ++it)
		{
			if (Tower::enemy_in_range(dynamic_cast<Enemy*>(*it), mExplosive_radius, this->get_coords()))
			{
				dynamic_cast<Enemy*>(*it)->take_damage(&mDamage);
			}
		}
		delete this;
	}
}

bool AoeShot::follow()
{
	bool enemy_hit = Shot::follow(mLocation_to_shoot);

	if (enemy_hit) {
		CoordinatesInDouble coordinates, direction;
		coordinates.x = mLocation_to_shoot.x + (rand() % 7 - 3);
		coordinates.y = mLocation_to_shoot.y + (rand() % 7 - 3);
		direction.x = (rand() % 31 - 15) / 40.0;
		direction.y = (rand() % 31 - 15) / 40.0;
		double rotation_speed = (rand() % 31 - 15) / 40.0;
		double rotation = rand() % 360;
		new Particle("explosion", coordinates, direction, rotation, rotation_speed);
	}
	return enemy_hit;
}

SDL_Point AoeShot::get_location_to_shoot() const
{
	return mLocation_to_shoot;
}
