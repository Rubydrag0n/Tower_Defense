#include "AoeShot.h"

AoeShot::AoeShot(Tower* tower, SDL_Point location_to_shoot) : Shot(tower)
{
	mLocation_to_shoot = location_to_shoot;
}


AoeShot::~AoeShot()
{

}

bool AoeShot::follow()
{
	return Shot::follow(mLocation_to_shoot);
}

SDL_Point AoeShot::get_location_to_shoot() const
{
	return mLocation_to_shoot;
}
