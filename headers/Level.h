#pragma once
#include "Wave.h"
#include <vector>
#include "Resources.h"

enum class TILETYPES
{
	EMPTY,
	BUILDING,
	STREET,
	PATH,
	WOOD,
	IRON,
	COUNT
};

class Level
{
public:
	Level(std::string level_number);
	~Level();

	//at the moment: update the first wave in the vector, if this wave is dead spawn the next wave
	void on_tick();

	bool is_dead() const;
	bool no_lives() const;

	int get_waves_count() const;
	std::vector<Wave>* get_waves();
	int get_lives() const;
	Resources* get_ressources();
	std::string get_level_number() const;

	void set_ressources(Resources ressources);
	void set_lives(int lives);
	TILETYPES** get_map_matrix();
	void set_map_matrix(int x, int y, TILETYPES type);
	
private:

	std::string mLevel_number;

	std::vector<Wave> mWaves;
	int mLives;
	Resources mRessources;
	int mWaves_count;
	TILETYPES** mMap_matrix;
};

