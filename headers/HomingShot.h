#pragma once
#include "Shot.h"

class HomingShot : 
	public Shot
{
public:
	HomingShot(Tower* tower, Enemy *enemy_to_shoot);
	~HomingShot();
	Enemy *get_enemy_to_shoot();
	bool follow();

private:
	Enemy *mEnemy_to_shoot;
};

