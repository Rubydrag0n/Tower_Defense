//Here comes the definiton of the game class
#include <stdio.h>
#include <iostream>

#include "Game.h"
#include "SDL_setup.h"
#include "Map.h"
#include "Tower.h"
#include "Level.h"
#include "Menu.h"
#include "IndustrialBuilding.h"
#include "HomingTower.h"
#include "AoeTower.h"
#include "MouseHandler.h"
#include "LayerHandler.h"
#include "EntityHandler.h"
#include "RenderableHandler.h"
#include "ConfigFile.h"
#include "Button.h"

void foo() {
	printf("foo got called!\n");
	return;
}

Game::Game()
{
	gMouse_handler = new MouseHandler();
	gEntity_handler = new EntityHandler();
	gRenderables_handler = new RenderableHandler();
	gOpen_tab = MENUTAB();
	mLevel = new Level("1");


	mMenu = new Menu(mLevel);

	mMap = new Map("level/Level1.FMP");

	SDL_Rect dim;
	dim.x = 500;
	dim.y = 500;
	dim.w = 100;
	dim.h = 26;
	auto testbutton = new Button("testbutton", dim, &foo);
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
	gRenderables_handler->render();
	mMap->render();
	mMenu->render(mMouse_position);
}

void Game::start_game()
{
	SDL_RenderClear(gRenderer);

	SDL_RenderPresent(gRenderer);


/*	auto testButton = new Button();
	std::string path = std::string(gConfig_file->Value("StartButton/sprite", "path"));
	testButton->mButtonSpriteSheetTexture = gTextures->get_texture(path);*/


	for (auto game_tick = 0; game_tick < 300000; game_tick++)
	{
		SDL_GetMouseState(&mMouse_position.x, &mMouse_position.y);
		//SDL_Delay(100);
		//also renders the hover window
		//mouse handler update needs to happen first
		gMouse_handler->update();
		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			gMouse_handler->handle_event(&e);
		}

		gEntity_handler->update();
		mLevel->on_tick();
		this->render_all();

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

