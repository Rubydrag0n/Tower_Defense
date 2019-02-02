#include "Level.h"
#include "Building.h"
#include "ConfigFile.h"
#include <fstream>

const int gMatrix_width = 20;
const int gMatrix_height = 16;

Level::Level(std::string level_number) : mLevel_number(level_number)
{
	mLives = gConfig_file->value_or_zero("level" + mLevel_number, "lives");
	mWaves_count = gConfig_file->value("level" + mLevel_number, "waves_count");

	//set the start-resources in this level
	mResources.set_resources(gConfig_file->value_or_zero("level" + mLevel_number, "gold"),
		gConfig_file->value_or_zero("level" + mLevel_number, "wood"),
		gConfig_file->value_or_zero("level" + mLevel_number, "stone"),
		gConfig_file->value_or_zero("level" + mLevel_number, "iron"),
		gConfig_file->value_or_zero("level" + mLevel_number, "energy"),
		gConfig_file->value_or_zero("level" + mLevel_number, "water"),
		gConfig_file->value_or_zero("level" + mLevel_number, "food"));

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

	this->mMap_matrix = new TILETYPES*[gMatrix_width];
	this->mMap_buildings = new Building**[gMatrix_width];
	for (auto i = 0; i < gMatrix_width; i++)
	{
		this->mMap_matrix[i] = new TILETYPES[gMatrix_height];
		this->mMap_buildings[i] = new Building*[gMatrix_height];
	}

	for (auto y = 0; y < gMatrix_height; y++)
	{
		file >> content;
		for (auto x = 0; x < gMatrix_width; x++)
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
	for (auto i = 0; i < gMatrix_width; i++)
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

Resources* Level::get_resources()
{
	return &mResources;
}

std::string Level::get_level_number() const
{
	return this->mLevel_number;
}


void Level::set_resources(const Resources* resources)
{
	mResources = *resources;
}

void Level::set_lives(const int lives)
{
	mLives = lives;
}

TILETYPES** Level::get_map_matrix() const
{
	return mMap_matrix;
}

void Level::set_map_matrix(const int x, const int y, const TILETYPES type) const
{
	mMap_matrix[x][y] = type;
}

void Level::set_building_matrix(const int x, const int y, Building* building) const
{
	this->mMap_buildings[x][y] = building;

	//set the neighbors of all buildings that change
	if (x > 0) {
		if (building != nullptr) building->set_neighbor(WEST, mMap_buildings[x - 1][y]);
		if (mMap_buildings[x - 1][y] != nullptr) mMap_buildings[x - 1][y]->set_neighbor(EAST, building);
	}
	if (y > 0) {
		if (building != nullptr) building->set_neighbor(NORTH, mMap_buildings[x][y - 1]);
		if (mMap_buildings[x][y - 1] != nullptr) mMap_buildings[x][y - 1]->set_neighbor(SOUTH, building);
	}
	if (x < gMatrix_width - 1) {
		if (building != nullptr) building->set_neighbor(EAST, mMap_buildings[x + 1][y]);
		if (mMap_buildings[x + 1][y] != nullptr) mMap_buildings[x + 1][y]->set_neighbor(WEST, building);
	}
	if (y < gMatrix_height - 1) {
		if (building != nullptr) building->set_neighbor(SOUTH, mMap_buildings[x][y + 1]);
		if (mMap_buildings[x][y + 1] != nullptr) mMap_buildings[x][y + 1]->set_neighbor(NORTH, building);
	}
}

Building* Level::get_building_matrix(const int x, const int y) const
{
	return this->mMap_buildings[x][y];
}

Warehouse* Level::get_main_building() const
{
	return this->mMain_building;
}

void Level::set_main_building(Warehouse *main_building) {
	this->mMain_building = main_building;
}
