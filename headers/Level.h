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
	bool isDead();
	bool noLives();

	int get_waves_count();
	std::vector<Wave>* get_waves();
	int get_lives();
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

