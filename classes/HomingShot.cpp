#include "HomingShot.h"
#include "Enemy.h"


HomingShot::HomingShot(Tower* tower, Enemy *enemy_to_shoot) : Shot(tower)
{
	mEnemy_to_shoot = enemy_to_shoot;
}


HomingShot::~HomingShot()
{
}

bool HomingShot::follow()
{
	return Shot::follow(mEnemy_to_shoot->get_position());
}


Enemy *HomingShot::get_enemy_to_shoot() const
{
	return mEnemy_to_shoot;
}

