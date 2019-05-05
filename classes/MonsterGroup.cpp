#include "MonsterGroup.h"
#include "ConfigFile.h"
#include "Level.h"
#include <iostream>
#include "SDL_setup.h"

MonsterGroup::MonsterGroup(const std::string& wave_number, const std::string& monster_group_number, Level* level) : mLevel(level)
{
	mElapsed_ticks = 0;
	mCurrent_monster_count = 0;
	const auto section = "monstergroup" + mLevel->get_level_number() + "_" + wave_number + "_" + monster_group_number;
	mSpawn_delay = gConfig_file->value(section, "spawn_delay");
	mSpawn_delay *= *gFrame_rate;

	mMonster_name.assign(gConfig_file->value(section, "monstername"));
	mDelay_ticks_between_monsters = gConfig_file->value(section, "delay_ticks");
	if (mDelay_ticks_between_monsters <= 0) mDelay_ticks_between_monsters = 1;
	mMax_monster_count = gConfig_file->value(section, "monster_count");
	mWay = gConfig_file->value(section, "way");
}

MonsterGroup::~MonsterGroup()
{
	//delete all the things in the vector in case there are any left
	for (auto& monster : mMonsters)
	{
		delete monster;
	}
	mMonsters.clear();
}

void MonsterGroup::update()
{
	if(mElapsed_ticks >= mSpawn_delay)
	{
		//whenever elapsed ticks is zero spawn new enemy copy
		if (mElapsed_ticks % mDelay_ticks_between_monsters == 0 && mCurrent_monster_count < mMax_monster_count)
		{
			const auto new_enemy = new Enemy(mMonster_name, mWay, mLevel, ENEMIES);
			mMonsters.push_back(new_enemy);
			mCurrent_monster_count++;
		}

		for (auto& monster : mMonsters)
		{
			monster->move();
		}

		//updating the current tick
	}
	mElapsed_ticks++;
}

bool MonsterGroup::is_dead() const
{
	for (auto monster : mMonsters)
	{
		if (!monster->is_dead()) {
			return false;
		}
	}
	return true;
}

std::vector<Enemy*> MonsterGroup::get_monsters() const
{
	return mMonsters;
}

