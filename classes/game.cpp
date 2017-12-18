//Here comes the definiton of the game class
#include "Game.h"
#include "SDL_setup.h"
#include "MonsterGroup.h"
#include "Map.h"
#include <stdio.h>
#include "Tower.h"
#include <iostream>
#include "Level.h"
#include "Menu.h"
#include "IndustrialBuilding.h"
#include "HomingTower.h"
#include "AoeTower.h"
#include "MouseHandler.h"
#include "LayerHandler.h"

Game::Game()
{
	gMouse_handler = new MouseHandler();
	mLevel = new Level("1");

	SDL_Point coords;
	coords.x = 600;
	coords.y = 600;
	auto archer_tower = new HomingTower("archer", coords, mLevel);
	add_tower(archer_tower);
	coords.x = 200;
	coords.y = 200;
	auto cannon_tower = new AoeTower("cannon", coords, mLevel);
	add_tower(cannon_tower);
	coords.x = 100;
	coords.y = 100;
	auto lumberjack = new IndustrialBuilding("lumberjack", coords, mLevel);
	add_industrial_building(lumberjack);

	mMenu = new Menu(mLevel);
	mMap = new Map("level/Level1.FMP");

	gLayer_handler = new LayerHandler();
}

Game::~Game()
{
	delete mMenu;
	delete mLevel;
	delete mMap;
	for (auto i = 0; i<mAll_towers.size(); i++)
	{
		delete mAll_towers.at(i);
	}
	for (auto i = 0; i<mAll_enemies.size(); i++)
	{
		delete mAll_enemies.at(i);
	}
	for (auto i = 0; i<mAll_industrial_buildings.size(); i++)
	{
		delete mAll_industrial_buildings.at(i);
	}
}

void Game::render_all()
{
	mMap->render();
	mMenu->render();
	mLevel->render();
	for (auto i = 0; i<mAll_towers.size(); i++)
	{
		mAll_towers.at(i)->render();
	}
	for (auto i = 0; i<mAll_industrial_buildings.size(); i++)
	{
		mAll_industrial_buildings.at(i)->render();
	}
}

void Game::update_all()
{
	mAll_enemies.clear();
	mLevel->update();
	//add all enemies: for every wave every monstergroup in the level
	for (auto n = 0; n < mLevel->get_waves_count(); n++)
	{
		for (auto m = 0; m < mLevel->get_waves()->at(n).get_monster_group_count(); m++)
		{
			add_enemies(mLevel->get_waves()->at(n).get_monster_groups()->at(m).get_monsters());
		}
	}

	for (auto i = 0; i<mAll_towers.size(); i++)
	{
		mAll_towers.at(i)->shoot(mAll_enemies);
	}
	
	for (auto i = 0; i<mAll_industrial_buildings.size(); i++)
	{
		//TODO: Olli! das ist totaler bullshit
		if (i % 60 == 0)
		{
			mAll_industrial_buildings.at(i)->update();
		}
	}
}

void Game::start_game()
{

	SDL_RenderClear(gRenderer);

	SDL_RenderPresent(gRenderer);
	
	for (auto i = 0; i < 300000; i++)
	{
		//SDL_Delay(100);
		SDL_RenderClear(gRenderer);
		render_all();
		update_all();
		//also renders the hover window
		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			gMouse_handler->handle_event(&e);
		}
		gMouse_handler->update();

		gLayer_handler->present();

		if (i % 60 == 0)
		{
			//printf("Second...\n");
		}
		if (mLevel->is_dead())
		{
			//std::cout << "all enemies dead" << std::endl;
			SDL_Delay(10000);
			break;
		}

		if (mLevel->no_lives())
		{
			//std::cout << "no lives" << std::endl;
			SDL_Delay(10000);
			break;
		}
	}
}

void Game::add_enemies(std::vector<Enemy*> enemies)
{
	while(!enemies.empty())
	{
		mAll_enemies.push_back(enemies.at(0));
		enemies.erase(enemies.begin());
	}
}

void Game::add_tower(Tower* tower)
{
	mAll_towers.push_back(tower);
}

void Game::add_industrial_building(IndustrialBuilding* industrial_building)
{
	mAll_industrial_buildings.push_back(industrial_building);
}

