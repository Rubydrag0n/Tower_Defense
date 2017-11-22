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

	bool follow(); //shot follows an enemy

private:
	Enemy* mEnemy_to_shoot;
};

