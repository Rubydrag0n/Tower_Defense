#pragma once
#include "Shot.h"

class Enemy;

class HomingShot : 
	public Shot
{
public:
	HomingShot(Tower* tower, Enemy *enemy_to_shoot);
	~HomingShot();
	Enemy *get_enemy_to_shoot() const;

	void on_tick() override;

	bool follow(); //shot follows an enemy, returns true when hit

	void self_destruct();

private:
	Enemy* mEnemy_to_shoot;

	//when this is set the shot will selfdestruct next tick
	bool mDelete_me;
};
