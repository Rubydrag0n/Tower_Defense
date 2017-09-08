#include "Level.h"
#include "ConfigFile.h"


Level::Level(std::string level_number)
{
	ConfigFile cf("config/game.cfg");
	mLives = cf.Value("level" + level_number, "lives");
	mStart_Ressources.set_ressources(cf.Value("level" + level_number, "gold"),
		cf.Value("level" + level_number, "wood"),
		cf.Value("level" + level_number, "stone"),
		cf.Value("level" + level_number, "iron"),
		cf.Value("level" + level_number, "energy"),
		cf.Value("level" + level_number, "water"), 
		cf.Value("level" + level_number, "food"));
	mWaves_Count = cf.Value("level" + level_number, "waves_count");
	for (auto i = 1; i <= mWaves_Count; i++)
	{
		std::string wave_number = std::to_string(i);
		auto new_wave = new Wave(level_number, wave_number, this);
		mWaves.push_back(*new_wave);
	}
}


Level::~Level()
{
}

void Level::update()
{
	mWaves.at(0).update();
	if(mWaves.at(0).isDead())
	{
		mWaves_Count--;
		mWaves.erase(mWaves.begin());
	}
}

void Level::render()
{
	if(mWaves.empty())
	{
		return;
	}
	mWaves.at(0).render();
}

bool Level::isDead()
{
	return mWaves.empty();
}

bool Level::noLives()
{
	return mLives <= 0;
}

int Level::get_lives()
{
	return mLives;
}



int Level::get_waves_count()
{
	return mWaves_Count;
}

std::vector<Wave>* Level::get_waves()
{
	return &mWaves;
}

void Level::set_lives(int lives)
{
	mLives = lives;
}



