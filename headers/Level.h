#pragma once
#include "Wave.h"
#include <vector>
#include "Ressources.h"


class Level
{
public:
	Level(std::string level_number);
	~Level();

	void update();
	void render();
	bool is_dead() const;
	bool no_lives() const;

	int get_waves_count() const;
	std::vector<Wave>* get_waves();
	int get_lives() const;
	Ressources* get_ressources();

	void set_ressources(Ressources ressources);
	void set_lives(int lives);

	std::string mLevel_number;
	


private:
	std::vector<Wave> mWaves;
	int mLives;
	Ressources mRessources;
	int mWaves_Count;
};

