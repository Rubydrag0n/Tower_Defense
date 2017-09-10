#pragma once
#include "MonsterGroup.h"

class Wave
{
public:
	Wave(std::string wave_number, Level* level);
	~Wave();

	void update();
	void render();
	bool isDead();

	int get_monster_group_count();
	std::vector<MonsterGroup>* get_monster_groups();

private:
	std::vector<MonsterGroup> mMonster_groups;
	int mMonster_group_count;
	Level* mLevel;
};

