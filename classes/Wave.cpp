#include "Wave.h"
#include "ConfigFile.h"
#include "Level.h"


Wave::Wave(const std::string& wave_number, Level* level) : mLevel(level)
{
	const auto section = "wave" + mLevel->get_level_number() + "_" + wave_number;

	mMonster_group_count = gConfig_file->value(section, "monster_group_count");//number of monster groups in the wave

	//create all monstergroups in this wave and insert them in the vector mMonster_groups
	for(auto i = 1; ; i++)
	{
		auto monster_group_number = std::to_string(i);
		if(!gConfig_file->value_exists("monstergroup" + mLevel->get_level_number() + "_" + wave_number + "_" + monster_group_number, "exists"))
		{
			break;
		}
		auto new_monster_group = new MonsterGroup(wave_number, monster_group_number, mLevel);
		mMonster_groups.push_back(new_monster_group);
	}
}

void Wave::update()
{
	mMonster_groups.at(0)->update();
	if(mMonster_groups.at(0)->is_dead())
	{
		delete mMonster_groups[0];
		mMonster_groups.erase(mMonster_groups.begin());
	}
}

bool Wave::is_dead() const
{
	return mMonster_groups.empty();
}


std::vector<MonsterGroup*>* Wave::get_monster_groups()
{
	return &mMonster_groups;
}


