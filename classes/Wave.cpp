#include "Wave.h"
#include "ConfigFile.h"


Wave::Wave(std::string level_number, std::string wave_number, Level* level)
{
	ConfigFile cf("config/game.cfg");
	mLevel = level;
	mMonster_group_count = cf.Value("wave" + level_number + wave_number, "monster_group_count");
	for(auto i = 1; i <= mMonster_group_count; i++)
	{
		std::string monster_group_number = std::to_string(i);
		auto new_monster_group = new MonsterGroup(level_number, wave_number, monster_group_number, mLevel);
		mMonster_groups.push_back(*new_monster_group);
	}
	
}


Wave::~Wave()
{
}

void Wave::update()
{
	mMonster_groups.at(0).update();
	if(mMonster_groups.at(0).isDead())
	{
		mMonster_group_count--;
		mMonster_groups.erase(mMonster_groups.begin());
	}
}

bool Wave::isDead()
{
	return mMonster_groups.empty();
}

void Wave::render()
{
	if(mMonster_groups.empty())
	{
		return;
	}
	mMonster_groups.at(0).render();
}

int Wave::get_monster_group_count()
{
	return mMonster_group_count;
}

std::vector<MonsterGroup>* Wave::get_monster_groups()
{
	return &mMonster_groups;
}


