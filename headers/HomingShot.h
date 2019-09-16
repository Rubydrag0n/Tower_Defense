#pragma once
#include "Shot.h"

class Enemy;

class HomingShot : 
	public Shot
{
public:
	HomingShot(Tower* tower, Enemy *enemy_to_shoot);

	Enemy *get_enemy_to_shoot() const;

	void on_tick() override;

	bool follow(); //shot follows an enemy, returns true when hit

private:
	Enemy* mEnemy_to_shoot;

	//when this is set the shot will self destruct next tick
	bool mDelete_me;
};
