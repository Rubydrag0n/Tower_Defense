#include "HomingShot.h"
#include "Enemy.h"

HomingShot::HomingShot(Tower* tower, Enemy *enemy_to_shoot) : Shot(tower)
{
	mEnemy_to_shoot = enemy_to_shoot;
	this->set_target(enemy_to_shoot->get_position());
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

void HomingShot::on_tick()
{
	if (this->mEnemy_to_shoot->is_dead())
	{
		delete this;
		return;
	}
	this->set_target(mEnemy_to_shoot->get_position());
	bool enemy_hit = this->follow();
	if (enemy_hit)
	{
		this->get_enemy_to_shoot()->take_damage(&mDamage);
		delete this;
	}
}

