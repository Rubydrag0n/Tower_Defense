#include "MonsterGroup.h"
#include "ConfigFile.h"
#include "Level.h"
#include <iostream>

MonsterGroup::MonsterGroup(std::string wave_number, std::string monster_group_number, Level* Level) : mMonsters()
{
	mLevel = Level;
	mElapsed_ticks = 0;
	mCurrent_monster_count = 0;
	auto section = "monstergroup" + mLevel->get_level_number() + "_" + wave_number + "_" + monster_group_number;

	mMonster_name.assign(gConfig_file->Value(section, "monstername"));
	mDelay_ticks = gConfig_file->Value(section, "delay_ticks");
	mMax_monster_count = gConfig_file->Value(section, "monster_count");
	mWay = gConfig_file->Value(section, "way");
}

MonsterGroup::~MonsterGroup()
{
	//delete all the things in the vector in case there are any left
	for (auto i = 0; i < mMonsters.size(); i++)
	{
		delete mMonsters[i];
	}
	mMonsters.clear();
}

void MonsterGroup::update()
{
	//whenever elapsed ticks is zero spawn new enemy copy
	if (mElapsed_ticks == 0 && mCurrent_monster_count < mMax_monster_count)
	{
		auto new_enemy = new Enemy(mMonster_name, mWay, mLevel);
		mMonsters.push_back(new_enemy);
		mCurrent_monster_count++;
	}

	for (auto i = 0; i < mMonsters.size(); i++)
	{
		mMonsters.at(i)->move();
	}

	//updating the current tick
	mElapsed_ticks++;
	if (mDelay_ticks != 0) mElapsed_ticks %= mDelay_ticks;
	else mElapsed_ticks = 0;
}

bool MonsterGroup::is_dead() const
{
	for (auto i = 0; i < mMonsters.size(); i++)
	{
		if (!mMonsters[i]->is_dead()) {
			return false;
		}
	}
	return true;
}

std::vector<Enemy*> MonsterGroup::get_monsters() const
{
	return mMonsters;
}

