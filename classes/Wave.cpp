#include "Wave.h"
#include "ConfigFile.h"
#include "Level.h"


Wave::Wave(std::string wave_number, Level* level)
{
	mLevel = level;
	mMonster_group_count = gConfig_file->Value("wave" + mLevel->get_level_number() + wave_number, "monster_group_count");//number of monstergroups in the wave

	//create all monstergroups in this wave and insert them in the vector mMonster_groups
	for(auto i = 1; i <= mMonster_group_count; i++)
	{
		auto monster_group_number = std::to_string(i);
		auto new_monster_group = new MonsterGroup(wave_number, monster_group_number, mLevel);
		mMonster_groups.push_back(*new_monster_group);
	}
}


Wave::~Wave()
{
}

void Wave::update()
{
	mMonster_groups.at(0).update();
	if(mMonster_groups.at(0).is_dead())
	{
		mMonster_group_count--;
		mMonster_groups.erase(mMonster_groups.begin());
	}
}

bool Wave::is_dead() const
{
	return mMonster_groups.empty();
}


int Wave::get_monster_group_count() const
{
	return mMonster_group_count;
}

std::vector<MonsterGroup>* Wave::get_monster_groups()
{
	return &mMonster_groups;
}


