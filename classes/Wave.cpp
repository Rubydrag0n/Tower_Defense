#include "Wave.h"
#include "ConfigFile.h"
#include "Level.h"
#include "SDL_setup.h"

Wave::Wave(const std::string& wave_number, Level* level) : mLevel(level)
{
	const auto section = "wave" + mLevel->get_level_number() + "_" + wave_number;

	mSpawn_delay = gConfig_file->value(section, "spawn_delay");
	mSpawn_delay *=	*gFrame_rate;
	mElapsed_ticks = 0;

	//mMonster_group_count = gConfig_file->value(section, "monster_group_count");//number of monster groups in the wave

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
	if(mElapsed_ticks >= mSpawn_delay)
	{
		for(auto i = 0; i < mMonster_groups.size(); i++)
		{
			mMonster_groups.at(i)->update();
		}
	}
	
	/*if(mMonster_groups.at(0)->is_dead())
	{
		delete mMonster_groups[0];
		mMonster_groups.erase(mMonster_groups.begin());
	}*/

	mElapsed_ticks++;
}

bool Wave::is_dead() const
{
	return mMonster_groups.empty();
}

std::vector<MonsterGroup*>* Wave::get_monster_groups()
{
	return &mMonster_groups;
}
