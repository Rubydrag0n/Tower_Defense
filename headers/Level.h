#pragma once
#include "Wave.h"
#include <vector>
#include "Resources.h"


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
	Ressources* get_ressources();
	std::string get_level_number() const;

	void set_ressources(Ressources ressources);
	void set_lives(int lives);
	
private:

	std::string mLevel_number;

	std::vector<Wave> mWaves;
	int mLives;
	Ressources mRessources;
	int mWaves_count;
};

