#pragma once
#include <string>
#include <vector>
#include "Enemy.h"

/*
 * Holds one Type of monster, an amount of these monsters and a time constant,
 * describing in what intervals they are spawning
 * Is used in "Wave"
 */
class MonsterGroup
{
public:
	MonsterGroup();
	~MonsterGroup();

private:
	//the amount of monsters coming in this group
	int mMonster_count;
	//to load the right stats etc. for the monsters
	std::string mMonster_name;
	//the delay between each new monster of this group
	int mDelay_ticks;
	//vector actually storing the generated monsters
	std::vector<Enemy> mMonsters;
};
