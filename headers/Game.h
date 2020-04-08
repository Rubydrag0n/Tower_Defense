#pragma once
#include <vector>
#include "Tower.h"
#include "IndustrialBuilding.h"
#include "MainMenu.h"

class Game 
{
public:
	Game();
	~Game();

	void start_game();
	void render_all() const;

	void add_tower(Tower* tower);
	void add_industrial_building(IndustrialBuilding* industrial_building);

	void load_level(int level_number);

	enum class STATE
	{
		MAIN_MENU,
		PLAYING,
		EXITING
	};

public:
	void set_state(Game::STATE state);

private:

	STATE mState;
	
	std::vector<Tower*> mAll_towers;
	std::vector<IndustrialBuilding*> mAll_industrial_buildings;
	
	Level* mCurrent_level;
	MainMenu* mMain_menu;

	SDL_Point mMouse_position;
};
