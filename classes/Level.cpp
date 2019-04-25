#include "Level.h"
#include "Building.h"
#include "ConfigFile.h"
#include "WareHouse.h"
#include <fstream>
#include <utility>
#include "SDL_setup.h"

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
	mMain_building = new Warehouse(gConfig_file->value(level_section, "main_building_name"), warehouse_coord, this, BUILDINGS, BUILDINGS);

	new Menu(this, LAYERS::BACKGROUND);
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

void Level::set_building_matrix(const int x, const int y, Building* building, const int x_size, const int y_size) const
{
	if (x < 0 || x + x_size > MATRIX_WIDTH || y < 0 || y + y_size > MATRIX_HEIGHT) return;

	//set all the fields occupied
	for (auto x_i = 0; x_i < x_size; x_i++)
		for (auto y_i = 0; y_i < y_size; y_i++)
			this->mMap_buildings[x + x_i][y + y_i] = building;

	//update building's neighbors
	//up side
	if (y > 0)
	{
		for (auto x_i = 0; x_i < x_size; x_i++)
		{
			if (get_building_matrix(x + x_i, y - 1) != nullptr)
				get_building_matrix(x + x_i, y - 1)->update_neighbors();
		}
	}

	//bottom side
	if (y < MATRIX_HEIGHT - y_size)
	{
		for (auto x_i = 0; x_i < x_size; x_i++)
		{
			if (get_building_matrix(x + x_i, y + y_size) != nullptr)
				get_building_matrix(x + x_i, y + y_size)->update_neighbors();
		}
	}

	//left side
	if (x > 0)
	{
		for (auto y_i = 0; y_i < y_size; y_i++)
		{
			if (get_building_matrix(x - 1, y + y_i) != nullptr)
				get_building_matrix(x - 1, y + y_i)->update_neighbors();
		}
	}

	//right side
	if (x < MATRIX_WIDTH - x_size)
	{
		for (auto y_i = 0; y_i < y_size; y_i++)
		{
			if (get_building_matrix(x + x_size, y + y_i) != nullptr)
				get_building_matrix(x + x_size, y + y_i)->update_neighbors();
		}
	}
}

Building* Level::get_building_matrix(const int x, const int y) const
{
	if (x < 0 || x >= MATRIX_WIDTH || y < 0 || y >= MATRIX_HEIGHT) return nullptr;
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
