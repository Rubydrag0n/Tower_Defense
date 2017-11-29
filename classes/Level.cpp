#include "Level.h"
#include "ConfigFile.h"


Level::Level(std::string level_number)
{
	ConfigFile cf("config/game.cfg");

	mLevel_number = level_number;
	mLives = cf.Value("level" + mLevel_number, "lives");
	mWaves_count = cf.Value("level" + mLevel_number, "waves_count");

	//set the start-ressources in this level
	mRessources.set_ressources(cf.Value("level" + mLevel_number, "gold"),
		cf.Value("level" + mLevel_number, "wood"),
		cf.Value("level" + mLevel_number, "stone"),
		cf.Value("level" + mLevel_number, "iron"),
		cf.Value("level" + mLevel_number, "energy"),
		cf.Value("level" + mLevel_number, "water"), 
		cf.Value("level" + mLevel_number, "food"));

	//create all waves in the level and insert them in the vector mWaves
	for (auto i = 1; i <= mWaves_count; i++)
	{
		auto wave_number = std::to_string(i);
		auto new_wave = new Wave(wave_number, this);
		mWaves.push_back(*new_wave);
	}
}


Level::~Level()
{
}


void Level::update()
{
	mWaves.at(0).update();
	if(mWaves.at(0).is_dead())
	{
		mWaves_count--;
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

bool Level::is_dead() const
{
	return mWaves.empty();
}

bool Level::no_lives() const
{
	return mLives <= 0;
}

int Level::get_lives() const
{
	return mLives;
}

int Level::get_waves_count() const
{
	return mWaves_count;
}

std::vector<Wave>* Level::get_waves()
{
	return &mWaves;
}

Ressources* Level::get_ressources()
{
	return &mRessources;
}

std::string Level::get_level_number() const
{
	return this->mLevel_number;
}


void Level::set_ressources(Ressources ressources)
{
	mRessources = ressources;
}

void Level::set_lives(int lives)
{
	mLives = lives;
}
