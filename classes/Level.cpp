#include "Level.h"
#include "Building.h"
#include "ConfigFile.h"
#include "WareHouse.h"
#include <fstream>
#include <utility>
#include "SDL_setup.h"

const int MATRIX_WIDTH = 20;
const int MATRIX_HEIGHT = 16;

Level::Level(std::string level_number) : mLevel_number(std::move(level_number)), mMain_building()
{
	//mLevel_number = level_number;
	auto level_section = "level" + mLevel_number;
	mLives = gConfig_file->value_or_zero(level_section, "lives");

	//set the start-resources in this level
	mStart_resources.set_resources(gConfig_file->value_or_zero("level" + mLevel_number, "gold"),
		gConfig_file->value_or_zero(level_section, "wood"),
		gConfig_file->value_or_zero(level_section, "stone"),
		gConfig_file->value_or_zero(level_section, "iron"),
		gConfig_file->value_or_zero(level_section, "energy"),
		gConfig_file->value_or_zero(level_section, "water"),
		gConfig_file->value_or_zero(level_section, "food"));

	//create all waves in the level and insert them in the vector mWaves
	for (auto i = 1; ; i++)
	{
		auto wave_number = std::to_string(i);
		if(!gConfig_file->value_exists("wave" + mLevel_number + "_" + wave_number, "exists"))
		{
			break;
		}
		auto new_wave = new Wave(wave_number, this);
		mWaves.push_back(new_wave);
	}

	//create Level-Matrix
	std::ifstream file("level/Level1.txt");
	std::string content;

	this->mMap_matrix = new TILETYPES*[MATRIX_WIDTH];
	this->mMap_buildings = new Building**[MATRIX_WIDTH];
	for (auto i = 0; i < MATRIX_WIDTH; i++)
	{
		this->mMap_matrix[i] = new TILETYPES[MATRIX_HEIGHT];
		this->mMap_buildings[i] = new Building*[MATRIX_HEIGHT];
	}

	for (auto y = 0; y < MATRIX_HEIGHT; y++)
	{
		file >> content;
		for (auto x = 0; x < MATRIX_WIDTH; x++)
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


	//has to happen after mMap_buildings was initialized
	SDL_Point warehouse_coord;
	warehouse_coord.x = TILE_WIDTH * gConfig_file->value(level_section, "main_building_x");
	warehouse_coord.y = TILE_HEIGHT * gConfig_file->value(level_section, "main_building_y");
	mMain_building = new Warehouse(gConfig_file->value(level_section, "main_building_name"), warehouse_coord, this);

	new Menu(this);
}

Level::~Level()
{
	for (auto i = 0; i < MATRIX_WIDTH; i++)
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

Resources* Level::get_resources() const
{
	return this->mMain_building->get_current_resources();
}

void Level::set_resources(const Resources* resources)
{
	mStart_resources = *resources;
}

std::string Level::get_level_number() const
{
	return this->mLevel_number;
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
	if (x < MATRIX_WIDTH - 1) {
		if (building != nullptr) building->set_neighbor(EAST, mMap_buildings[x + 1][y]);
		if (mMap_buildings[x + 1][y] != nullptr) mMap_buildings[x + 1][y]->set_neighbor(WEST, building);
	}
	if (y < MATRIX_HEIGHT - 1) {
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
	this->mMain_building->get_current_resources()->set_empty();
	this->mMain_building->get_current_resources()->add(&this->mStart_resources);
}
