#include "HomingShot.h"
#include "Enemy.h"
#include "EntityHandler.h"
#include "Tower.h"

HomingShot::HomingShot(Tower* tower, Enemy *enemy_to_shoot) : Shot(tower), mDelete_me{false}
{
	mEnemy_to_shoot = enemy_to_shoot;
	set_target(enemy_to_shoot->get_position());
	//enemy_to_shoot->add_following_shot(this);
}

bool HomingShot::follow()
{
	return Shot::follow(mTarget);
}

Enemy *HomingShot::get_enemy_to_shoot() const
{
	return mEnemy_to_shoot;
}

void HomingShot::on_tick()
{
	if(mEnemy_to_shoot != nullptr) set_target(mEnemy_to_shoot->get_position());
	if (Shot::follow(mTarget))
	{
		if (mEnemy_to_shoot != nullptr && mExplosive_radius == 0)
		{
			mEnemy_to_shoot->take_damage(&mDamage);
		}
		else
		{
			damaged_an_enemy();
		}
		delete this;
		
		//if enemy didn't die delete shot yourself
		//otherwise all shots will be deleted by the enemy on death
		//if (!get_enemy_to_shoot()->take_damage(&mDamage)) get_enemy_to_shoot()->delete_following_shot(this);
	}
}
