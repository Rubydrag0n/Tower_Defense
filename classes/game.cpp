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
#include "EntityHandler.h"

Game::Game()
{
	gMouse_handler = new MouseHandler();
	gEntity_handler = new EntityHandler();
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
	MenuItem* testItem = new MenuItem("archer", mLevel);
	mMenu->add_menu_item(testItem);
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
	for (auto i = 0; i<mAll_industrial_buildings.size(); i++)
	{
		delete mAll_industrial_buildings.at(i);
	}
}

void Game::render_all()
{
	mMap->render();
	mMenu->render(mMouse_position);
}

void Game::start_game()
{
	SDL_RenderClear(gRenderer);

	SDL_RenderPresent(gRenderer);
	
	for (auto game_tick = 0; game_tick < 300000; game_tick++)
	{
		SDL_GetMouseState(&mMouse_position.x, &mMouse_position.y);
		//SDL_Delay(100);
		//also renders the hover window
		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			gMouse_handler->handle_event(&e);
		}
		gMouse_handler->update();

		gEntity_handler->update();
		mLevel->on_tick();
		gEntity_handler->render();
		render_all();

		gLayer_handler->present();

		if (game_tick % 60 == 0)
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

void Game::add_tower(Tower* tower)
{
	mAll_towers.push_back(tower);
}

void Game::add_industrial_building(IndustrialBuilding* industrial_building)
{
	mAll_industrial_buildings.push_back(industrial_building);
}

