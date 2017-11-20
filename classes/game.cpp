//Here comes the definiton of the game class
#include "Game.h"
#include "SDL_setup.h"
#include "MonsterGroup.h"
#include "Map.h"
#include <stdio.h>
#include "Tower.h"
#include <iostream>
#include "Level.h"
#include "Button.h"
#include "Menu.h"
#include "IndustrialBuilding.h"
#include "HomingTower.h"
#include "AoeTower.h"
#include "MouseHandler.h"
#include "Clickable.h"

Game::Game()
{
}

Game::~Game()
{
}

void Game::init_game()
{
	gMouse_handler = new MouseHandler();
}

void Game::start_game()
{
	auto test_map = new Map("level/Level1.FMP");

	SDL_RenderClear(gRenderer);

	test_map->render();

	SDL_RenderPresent(gRenderer);
	SDL_Point coords;
	coords.x = 600;
	coords.y = 600;
	
	auto test_button = new Button();
	test_button->set_position(500, 500);
	test_button->set_dimension(300, 100);
	auto test_level = new Level("1");
	auto test_aoe_tower1 = new AoeTower("cannon", coords, test_level);
	gMouse_handler->add_clickable(test_aoe_tower1);
	coords.x = 200;
	coords.y = 200;
	auto test_homing_tower = new HomingTower("archer", coords, test_level);
	gMouse_handler->add_clickable(test_homing_tower);
	auto test_menu = new Menu(test_level);

	coords.x = 100;
	coords.y = 100;
	auto test_industrial_building = new IndustrialBuilding("lumberjack", coords, test_level);
	gMouse_handler->add_clickable(test_industrial_building);

	for (auto i = 0; i < 300000; i++)
	{
		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			gMouse_handler->handle_event(&e);
		}

		//SDL_Delay(100);
		mAll_enemies.clear();
		test_level->update();
		//add all enemies: for every wave every monstergroup in the level
		for(auto n = 0; n < test_level->get_waves_count(); n++)
		{
			for(auto m = 0; m < test_level->get_waves()->at(n).get_monster_group_count(); m++)
			{
				add_enemies(test_level->get_waves()->at(n).get_monster_groups()->at(m).get_monsters());
			}
		}
		test_aoe_tower1->update(mAll_enemies);
		test_homing_tower->update(mAll_enemies);
		
		SDL_RenderClear(gRenderer);
		test_map->render();
		test_level->render();
		test_aoe_tower1->render();
		test_homing_tower->render();
		test_menu->render();
		test_button->render();
		test_industrial_building->render();
		SDL_RenderPresent(gRenderer);
		if (i % 60 == 0)
		{
			//printf("Second...\n");
			test_industrial_building->update();
		}
		if (test_level->is_dead())
		{
			//std::cout << "all enemies dead" << std::endl;
			SDL_Delay(10000);
			break;
		}

		if (test_level->no_lives()) 
		{
			//std::cout << "no lives" << std::endl;
			SDL_Delay(10000);
			break;
		}
	}
	delete test_aoe_tower1;
	delete test_homing_tower;
	delete test_menu;
	delete test_level;
	delete test_map;
	delete test_button;
	delete test_industrial_building;
}

void Game::add_enemies(std::vector<Enemy*> enemies)
{
	while(!enemies.empty())
	{
		mAll_enemies.push_back(enemies.at(0));
		enemies.erase(enemies.begin());
	}
}

void Game::add_tower(Tower tower)
{
	mAll_towers.push_back(tower);
}

