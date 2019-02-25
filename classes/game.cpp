//Here comes the definition of the game class
#include <cstdio>
#include <iostream>

#include "Game.h"
#include "SDL_setup.h"
#include "Map.h"
#include "Tower.h"
#include "Level.h"
#include "Menu.h"
#include "IndustrialBuilding.h"
#include "HomingTower.h"
#include "MouseHandler.h"
#include "LayerHandler.h"
#include "EntityHandler.h"
#include "RenderableHandler.h"
#include "ConfigFile.h"
#include "Carriage.h"
#include "WareHouse.h"
#include "Path.h"

Game::Game() : mMouse_position()
{
	gMouse_handler = new MouseHandler();
	gEntity_handler = new EntityHandler();
	gRenderables_handler = new RenderableHandler();
	mLevel = new Level("1");


	mMenu = new Menu(mLevel);


	mMap = new Map(const_cast<char*>("level/Level1.FMP"));

	gLayer_handler = new LayerHandler();

	SDL_Point p;
	p.x = 896;
	p.y = 448;

	p.x += 64;
	new Path("path", p, mLevel);
	p.x += 64;
	new Path("path", p, mLevel);
	p.x += 64;
	new Path("path", p, mLevel);
	p.x += 64;
	new Path("path", p, mLevel);
	p.y += 64;
	new Path("path", p, mLevel);
	p.y += 64;
	new Path("path", p, mLevel);

	p.x = 1152;
	p.y = 640;
	const auto warehouse = new Warehouse("warehouse", p, mLevel);
	this->mLevel->set_main_building(warehouse);

	p.x = 896;
	p.y = 448;

	new HomingTower("archer", p, mLevel);

	const auto r = new Resources(1000, 500, 200, 200, 0, 0, 2000);
	warehouse->add_resources(r);

}

Game::~Game()
{
	delete mMenu;
	delete mLevel;
	delete mMap;
	for (auto& tower : mAll_towers)
	{
		delete tower;
	}
	for (auto& building : mAll_industrial_buildings)
	{
		delete building;
	}
}

void Game::render_all() const
{
	gRenderables_handler->render();
	mMap->render();
}

void Game::start_game()
{
	SDL_RenderClear(gRenderer);

	SDL_RenderPresent(gRenderer);

	for (auto game_tick = 0; game_tick < 300000; game_tick++)
	{
		//also renders the hover window
		//mouse handler update needs to happen first
		gMouse_handler->update();
		gMouse_handler->get_mouse_position(&mMouse_position.x, &mMouse_position.y);
		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			gMouse_handler->handle_event(&e);
		}

		gEntity_handler->update();
		mLevel->on_tick();
		this->render_all();

		gLayer_handler->present();

		if (mLevel->is_dead() || mLevel->no_lives())
		{
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
