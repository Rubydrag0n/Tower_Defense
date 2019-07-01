//Here comes the definition of the game class
#include <cstdio>
#include <iostream>

#include "Game.h"
#include "SDL_setup.h"
#include "Map.h"
#include "Tower.h"
#include "Level.h"
#include "IndustrialBuilding.h"
#include "MouseHandler.h"
#include "LayerHandler.h"
#include "EntityHandler.h"
#include "RenderableHandler.h"
#include "ConfigFile.h"
#include "Timer.h"

Game::Game() : mState(STATE::MAIN_MENU), mMouse_position()
{
	mMain_menu = new MainMenu(this);
	mCurrent_level = nullptr;
}

Game::~Game()
{
	for (auto& tower : mAll_towers)
	{
		delete tower;
	}
	for (auto& building : mAll_industrial_buildings)
	{
		delete building;
	}
	delete mCurrent_level;
}

void Game::render_all() const
{
	gRenderables_handler->render();
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

		//Calculate and correct fps
		/* Not used right now
		auto avg_fps = counted_frames / (fps_timer.get_ticks() / 1000.f);
		if (avg_fps > 2000000) avg_fps = 0;
		*/

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

		gEntity_handler->update();

		switch(mState)
		{
		case STATE::MAIN_MENU:
			break;
		case STATE::PLAYING:
			mCurrent_level->on_tick();

			if (mCurrent_level->is_dead() || mCurrent_level->no_lives())
			{
				mState = STATE::MAIN_MENU;
				delete mCurrent_level;
			}
			break;
		default: ;
		}

		this->render_all();

		gLayer_handler->present();


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

void Game::load_level(const int level_number)
{
	if (!gConfig_file->value_exists("level" + std::to_string(level_number), "exists"))
	{
		printf("Could not load level, doesn't exist in config file!\n");
		return;
	}
	mCurrent_level = new Level(std::to_string(level_number), this);
}

void Game::set_state(const Game::STATE state)
{
	mState = state;
	switch(state)
	{
	case STATE::MAIN_MENU:
		mMain_menu->set_enabled(true);
		break;
	case STATE::PLAYING: 
		mMain_menu->set_enabled(false);
		break;
	default: ;
	}
}
