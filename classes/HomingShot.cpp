#include "HomingShot.h"
#include "Enemy.h"

HomingShot::HomingShot(Tower* tower, Enemy *enemy_to_shoot) : Shot(tower), mDelete_me{false}
{
	mEnemy_to_shoot = enemy_to_shoot;
	this->set_target(enemy_to_shoot->get_position());
	enemy_to_shoot->add_following_shot(this);
}

bool HomingShot::follow()
{
	return Shot::follow(mEnemy_to_shoot->get_position());
}

void HomingShot::self_destruct()
{
	this->mDelete_me = true;
}

Enemy *HomingShot::get_enemy_to_shoot() const
{
	return mEnemy_to_shoot;
}

void HomingShot::on_tick()
{
	if (mDelete_me) {
		delete this;
		return;
	}

	this->set_target(mEnemy_to_shoot->get_position());

	if (this->follow())
	{
		//if enemy didn't die delete shot yourself
		//otherwise all shots will be deleted by the enemy on death
		if (!this->get_enemy_to_shoot()->take_damage(&mDamage)) {
			this->get_enemy_to_shoot()->delete_following_shot(this);
		}
	}
	if (mEnemy_to_shoot->is_dead()) delete mEnemy_to_shoot;
}
