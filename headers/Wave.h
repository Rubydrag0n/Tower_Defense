#pragma once
#include "MonsterGroup.h"

class Wave
{
public:
	Wave(std::string wave_number, Level* level);
	~Wave();

	//at the moment: update the first monstergroup in the vector, if it is dead spawn the next wave
	void update();

	//Wave::update deletes mMonstergroup.at(0), if the monstergroup is dead, so the next monstergroup will be rendered
	bool is_dead() const;

	int get_monster_group_count() const;
	std::vector<MonsterGroup>* get_monster_groups();

private:
	std::vector<MonsterGroup> mMonster_groups;
	int mMonster_group_count;
	Level* mLevel;
};

