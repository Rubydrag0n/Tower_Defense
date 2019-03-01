#include "Level.h"
#include "ConfigFile.h"
#include <iostream>
#include <fstream>

const int matrix_width = 20;
const int matrix_height = 16;

Level::Level(std::string level_number)
{
	mLevel_number = level_number;
	mLives = gConfig_file->value_or_zero("level" + mLevel_number, "lives");

	//set the start-ressources in this level
	mRessources.set_resources(gConfig_file->value_or_zero("level" + mLevel_number, "gold"),
		gConfig_file->value_or_zero("level" + mLevel_number, "wood"),
		gConfig_file->value_or_zero("level" + mLevel_number, "stone"),
		gConfig_file->value_or_zero("level" + mLevel_number, "iron"),
		gConfig_file->value_or_zero("level" + mLevel_number, "energy"),
		gConfig_file->value_or_zero("level" + mLevel_number, "water"),
		gConfig_file->value_or_zero("level" + mLevel_number, "food"));

	//create all waves in the level and insert them in the vector mWaves
	for (auto i = 1; ; i++)
	{
		auto wave_number = std::to_string(i);
		if(!gConfig_file->value_exists("wave" + level_number + "_" + wave_number, "exists"))
		{
			break;
		}
		auto new_wave = new Wave(wave_number, this);
		mWaves.push_back(new_wave);
	}

	//create Level-Matrix
	std::ifstream file("level/Level1.txt");
	std::string content;

	this->mMap_matrix = new TILETYPES*[matrix_width];
	this->mMap_buildings = new Building**[matrix_width];
	for (auto i = 0; i < matrix_width; i++)
	{
		this->mMap_matrix[i] = new TILETYPES[matrix_height];
		this->mMap_buildings[i] = new Building*[matrix_height];
	}

	for (auto y = 0; y < matrix_height; y++)
	{
		file >> content;
		for (auto x = 0; x < matrix_width; x++)
		{
			switch (content.at(x))
			{
			case '0':
				mMap_matrix[x][y] = TILETYPES::EMPTY;
				break;
			case '1':
				mMap_matrix[x][y] = TILETYPES::BUILDINGTILE;
				break;
			case '2':
				mMap_matrix[x][y] = TILETYPES::STREETTILE;
				break;
			case '3':
				mMap_matrix[x][y] = TILETYPES::MONSTERPATH;
				break;
			case '4':
				mMap_matrix[x][y] = TILETYPES::WOODTILE;
				break;
			case '5':
				mMap_matrix[x][y] = TILETYPES::IRONTILE;
				break;
			default:
				break;
			}
			this->mMap_buildings[x][y] = nullptr;
		}
	}
}

Level::~Level()
{
	for (auto i = 0; i < matrix_width; i++)
	{
		delete this->mMap_matrix[i];
	}
	delete this->mMap_matrix;
}

void Level::on_tick()
{
	mWaves.at(0)->update();
	if(mWaves.at(0)->is_dead())
	{
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


std::vector<Wave*> Level::get_waves()
{
	return mWaves;
}

Resources* Level::get_resources()
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

void Level::set_building_matrix(int x, int y, Building* building)
{
	this->mMap_buildings[x][y] = building;

	//set the neighbours of all buildings that change
	if (x > 0) {
		if (building != nullptr) building->set_neighbour(WEST, mMap_buildings[x - 1][y]);
		if (mMap_buildings[x - 1][y] != nullptr) mMap_buildings[x - 1][y]->set_neighbour(EAST, building);
	}
	if (y > 0) {
		if (building != nullptr) building->set_neighbour(NORTH, mMap_buildings[x][y - 1]);
		if (mMap_buildings[x][y - 1] != nullptr) mMap_buildings[x][y - 1]->set_neighbour(SOUTH, building);
	}
	if (x < matrix_width - 1) {
		if (building != nullptr) building->set_neighbour(EAST, mMap_buildings[x + 1][y]);
		if (mMap_buildings[x + 1][y] != nullptr) mMap_buildings[x + 1][y]->set_neighbour(WEST, building);
	}
	if (y < matrix_height - 1) {
		if (building != nullptr) building->set_neighbour(SOUTH, mMap_buildings[x][y + 1]);
		if (mMap_buildings[x][y + 1] != nullptr) mMap_buildings[x][y + 1]->set_neighbour(NORTH, building);
	}
}

Building* Level::get_building_matrix(int x, int y)
{
	return this->mMap_buildings[x][y];
}
