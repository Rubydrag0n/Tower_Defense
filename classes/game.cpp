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
#include "Timer.h"

Game::Game() : mMouse_position()
{
	gMouse_handler = new MouseHandler();
	gEntity_handler = new EntityHandler();
	gRenderables_handler = new RenderableHandler();

	for(auto i = 1; ; i++)
	{
		if(!gConfig_file->value_exists("level" + std::to_string(i), "exists"))
		{
			break;
		}
		auto new_level = new Level(std::to_string(i));
		mLevels.push_back(new_level);
	}
	mCurrent_level = mLevels.at(0);


	mMap = new Map(const_cast<char*>("level/Level1.FMP"));

	gLayer_handler = new LayerHandler();

	SDL_Point p;
	p.x = 896;
	p.y = 448;

	auto tower = new HomingTower("archer", p, mCurrent_level, BUILDINGS, BUILDINGS);

	p.x += 64;
	new Path("path", p, mCurrent_level, BUILDINGS, BUILDINGS);
	p.x += 64;
	new Path("path", p, mCurrent_level, BUILDINGS, BUILDINGS);
	p.x += 64;
	new Path("path", p, mCurrent_level, BUILDINGS, BUILDINGS);
	p.x += 64;
	new Path("path", p, mCurrent_level, BUILDINGS, BUILDINGS);
	p.y += 64;
	new Path("path", p, mCurrent_level, BUILDINGS, BUILDINGS);
	p.y += 64;
	new Path("path", p, mCurrent_level, BUILDINGS, BUILDINGS);

	/*
	p.x = 1152;
	p.y = 640;
	const auto warehouse = new Warehouse("warehouse", p, mLevel);
	this->mLevel->set_main_building(warehouse);
	*/

	p.x = 896;
	p.y = 448;

	//new HomingTower("archer", p, mCurrent_level);

	const auto r = new Resources(1000, 500, 200, 200, 0, 0, 2000);
	mCurrent_level->get_main_building()->add_resources(r);
}

Game::~Game()
{
	for(auto i = 0; i < mLevels.size(); i++)
	{
		delete mLevels.at(i);
	}
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

	//loop flag
	auto quit = false;

	//frames per second timer
	Timer fps_timer;

	//the frames per second cap timer
	Timer cap_timer;

	const auto counted_frames = 0;
	fps_timer.start();

	while (!quit)
	{
		cap_timer.start();

		//also renders the hover window
		//mouse handler update needs to happen first
		gMouse_handler->update();
		gMouse_handler->get_mouse_position(&mMouse_position.x, &mMouse_position.y);
		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			gMouse_handler->handle_event(&e);

			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
		}

		//Calculate and correct fps
		auto avg_fps = counted_frames / (fps_timer.get_ticks() / 1000.f);
		if (avg_fps > 2000000) avg_fps = 0;

		gEntity_handler->update();
		mCurrent_level->on_tick();
		this->render_all();

		gLayer_handler->present();

		if (mCurrent_level->is_dead() || mCurrent_level->no_lives())
		{
			SDL_Delay(10000);
			quit = true;
		}

		//if frame finished early
		const auto frame_ticks = cap_timer.get_ticks();
		if (frame_ticks < *gTicks_per_frame)
		{
			//wait remaining time
			SDL_Delay(*gTicks_per_frame - frame_ticks);
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
