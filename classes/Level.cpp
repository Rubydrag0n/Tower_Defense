#include "Level.h"
#include "ConfigFile.h"


Level::Level(std::string level_number)
{
	ConfigFile cf("config/game.cfg");

	mLevel_number = level_number;
	mLives = cf.Value("level" + mLevel_number, "lives");
	mWaves_Count = cf.Value("level" + mLevel_number, "waves_count");

	//set the start-ressources in this level
	mRessources.set_ressources(cf.Value("level" + mLevel_number, "gold"),
		cf.Value("level" + mLevel_number, "wood"),
		cf.Value("level" + mLevel_number, "stone"),
		cf.Value("level" + mLevel_number, "iron"),
		cf.Value("level" + mLevel_number, "energy"),
		cf.Value("level" + mLevel_number, "water"), 
		cf.Value("level" + mLevel_number, "food"));

	//create all waves in the level and insert them in the vector mWaves
	for (auto i = 1; i <= mWaves_Count; i++)
	{
		std::string wave_number = std::to_string(i);
		auto new_wave = new Wave(wave_number, this);
		mWaves.push_back(*new_wave);
	}
}


Level::~Level()
{
}

//at the moment: update the first wave in the vector, if this wave is dead spawn the next wave
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

Ressources Level::get_ressources()
{
	return mRessources;
}


void Level::set_ressources(Ressources ressources)
{
	mRessources = ressources;
}

void Level::set_lives(int lives)
{
	mLives = lives;
}



