#include "Level.h"
#include "ConfigFile.h"
#include <iostream>
#include <fstream>


Level::Level(std::string level_number)
{
	mLevel_number = level_number;
	mLives = gConfig_file->Value("level" + mLevel_number, "lives");
	mWaves_count = gConfig_file->Value("level" + mLevel_number, "waves_count");

	//set the start-ressources in this level
	mRessources.set_resources(gConfig_file->Value("level" + mLevel_number, "gold"),
		gConfig_file->Value("level" + mLevel_number, "wood"),
		gConfig_file->Value("level" + mLevel_number, "stone"),
		gConfig_file->Value("level" + mLevel_number, "iron"),
		gConfig_file->Value("level" + mLevel_number, "energy"),
		gConfig_file->Value("level" + mLevel_number, "water"), 
		gConfig_file->Value("level" + mLevel_number, "food"));

	//create all waves in the level and insert them in the vector mWaves
	for (auto i = 1; i <= mWaves_count; i++)
	{
		auto wave_number = std::to_string(i);
		auto new_wave = new Wave(wave_number, this);
		mWaves.push_back(*new_wave);
	}

	//create Level-Matrix
	std::ifstream file("level/Level1.txt");
	std::string content;

	this->mMap_matrix = new TILETYPES*[20];
	for (auto i = 0; i < 20; i++)
	{
		this->mMap_matrix[i] = new TILETYPES[16];
	}


	for (auto y = 0; y < 16; y++)
	{
		file >> content;
		for (auto x = 0; x < 20; x++)
		{
			switch (content.at(x))
			{
			case '0':
				mMap_matrix[x][y] = TILETYPES::EMPTY;
				break;
			case '1':
				mMap_matrix[x][y] = TILETYPES::BUILDING;
				break;
			case '2':
				mMap_matrix[x][y] = TILETYPES::STREET;
				break;
			case '3':
				mMap_matrix[x][y] = TILETYPES::PATH;
				break;
			case '4':
				mMap_matrix[x][y] = TILETYPES::WOOD;
				break;
			default:
				break;
			}
		}
	}

}


Level::~Level()
{
	for (auto i = 0; i < 20; i++)
	{
		delete this->mMap_matrix[i];
	}
	delete this->mMap_matrix;
}


void Level::on_tick()
{
	mWaves.at(0).update();
	if(mWaves.at(0).is_dead())
	{
		mWaves_count--;
		mWaves.erase(mWaves.begin());
	}
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

Resources* Level::get_ressources()
{
	return &mRessources;
}

std::string Level::get_level_number() const
{
	return this->mLevel_number;
}


void Level::set_ressources(Resources ressources)
{
	mRessources = ressources;
}

void Level::set_lives(int lives)
{
	mLives = lives;
}

TILETYPES** Level::get_map_matrix()
{
	return mMap_matrix;
}

void Level::set_map_matrix(int x, int y, TILETYPES type)
{
	mMap_matrix[x][y] = type;
}


