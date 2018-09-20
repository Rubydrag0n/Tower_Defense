#include <SDL.h>

#include "AoeShot.h"
#include "Particles.h"

AoeShot::AoeShot(Tower* tower, SDL_Point location_to_shoot) : Shot(tower)
{
	mLocation_to_shoot = location_to_shoot;
}


AoeShot::~AoeShot()
{

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
