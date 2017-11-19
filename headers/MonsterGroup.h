#pragma once
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
	MonsterGroup(std::string wave_number, std::string monster_group_number, Level* Level);
	~MonsterGroup();

	//updates the monster group: looks if anyone has died, spawns new monsters in the given intervals
	//also moves all its monsters
	void update();
	//renders all the monsters in this group
	void render();
	//returns if every unit of this monstergroup
	bool is_dead() const;

	std::vector<Enemy*> get_monsters() const;

private:
	//the amount of monsters coming in this group
	int mMax_monster_count;
	//the amount already spawned
	int mCurrent_monster_count;
	//counts the already killed monsters
	int mDead_monsters;
	//to load the right stats etc. for the monsters
	std::string mMonster_name;
	int mWay;
	//the delay between each new monster of this group
	int mDelay_ticks;
	int mElapsed_ticks;
	//vector actually storing the generated monsters
	std::vector<Enemy*> mMonsters;
	Level* mLevel;
};
