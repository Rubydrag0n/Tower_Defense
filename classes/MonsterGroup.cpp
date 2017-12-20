#include "MonsterGroup.h"
#include "ConfigFile.h"
#include "Level.h"
#include <iostream>

MonsterGroup::MonsterGroup(std::string wave_number, std::string monster_group_number, Level* Level) : mMonsters()
{
	mLevel = Level;
	mElapsed_ticks = 0;
	mCurrent_monster_count = 0;
	mDead_monsters = 0;

	mMonster_name.assign(gConfig_file->Value("monstergroup" + mLevel->get_level_number() + wave_number + monster_group_number, "monstername"));
	mDelay_ticks = gConfig_file->Value("monstergroup" + mLevel->get_level_number() + wave_number + monster_group_number, "delay_ticks");
	mMax_monster_count = gConfig_file->Value("monstergroup" + mLevel->get_level_number() + wave_number + monster_group_number, "monster_count");
	mWay = gConfig_file->Value("monstergroup" + mLevel->get_level_number() + wave_number + monster_group_number, "way");
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
		if (mMonsters.at(i)->is_dead())
		{
			//the monster died -> delete from the vector
			delete mMonsters.at(i);
			mMonsters.erase(mMonsters.begin() + i);
			i--;
			mDead_monsters++;
			continue;
		}
		mMonsters.at(i)->move();
	}

	//updating the current tick
	mElapsed_ticks++;
	if (mDelay_ticks != 0) mElapsed_ticks %= mDelay_ticks;
	else mElapsed_ticks = 0;
}

bool MonsterGroup::is_dead() const
{
	//if every monsters of this group is dead:
	if (mDead_monsters == mMax_monster_count)
	{
		return true;
	}
	//otherwise
	return false;
}

std::vector<Enemy*> MonsterGroup::get_monsters() const
{
	return mMonsters;
}

