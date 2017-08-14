#include "MonsterGroup.h"

MonsterGroup::MonsterGroup(std::string monster_name, std::string level, int way, int delay_ticks, int monster_count) : mMonsters()
{
	mMonster_name = monster_name;
	mDelay_ticks = delay_ticks;
	mElapsed_ticks = 0;
	mLevel = level;
	mMax_monster_count = monster_count;
	mCurrent_monster_count = 0;
	mWay = way;
	mDead_monsters = 0;
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
		auto new_enemy = new Enemy(mMonster_name, mLevel, mWay);
		mMonsters.push_back(new_enemy);
		mCurrent_monster_count++;
	}

	for (auto i = 0; i < mMonsters.size(); i++)
	{
		if (mMonsters.at(i)->isDead())
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

void MonsterGroup::render()
{
	for (auto i = 0; i < mMonsters.size(); i++)
	{
		mMonsters.at(i)->render();
	}
}

bool MonsterGroup::isDead()
{
	//if every monsters of this group is dead:
	if (mDead_monsters == mMax_monster_count)
	{
		return true;
	}
	//otherwise
	return false;
}

std::vector<Enemy*> MonsterGroup::get_monsters()
{
	return mMonsters;
}

