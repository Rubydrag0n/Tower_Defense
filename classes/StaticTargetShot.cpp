#include <SDL.h>
#include "EntityHandler.h"
#include "StaticTargetShot.h"
#include "Particles.h"
#include "Tower.h"

StaticTargetShot::StaticTargetShot(Tower* tower, const SDL_Point location_to_shoot) : Shot(tower)
{
	//target is set once the shot exists, it wont change
	mTarget = location_to_shoot;
}

void StaticTargetShot::on_tick()
{
	if (follow())
	{
		damaged_an_enemy();
		delete this;
	}
}

bool StaticTargetShot::follow()
{
	const auto target_reached = Shot::follow(mTarget);

	if (target_reached) {
		CoordinatesInDouble coordinates, direction;
		coordinates.x = mTarget.x + (rand() % 7 - 3);
		coordinates.y = mTarget.y + (rand() % 7 - 3);
		direction.x = (rand() % 31 - 15) / 40.0;
		direction.y = (rand() % 31 - 15) / 40.0;
		const auto rotation_speed = (rand() % 31 - 15) / 40.f;
		const auto rotation = float(rand() % 360);
		new Particle("explosion", coordinates, direction, rotation, rotation_speed);
	}
	return target_reached;
}
