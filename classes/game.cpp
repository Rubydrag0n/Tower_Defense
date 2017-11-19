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


Game::Game()
{
}

Game::~Game()
{
}

void Game::start_game()
{
	auto testMap = new Map("level/Level1.FMP");

	SDL_RenderClear(gRenderer);

	testMap->render();

	SDL_RenderPresent(gRenderer);
	SDL_Point coords;
	coords.x = 600;
	coords.y = 600;
	
	auto testButton = new Button();
	testButton->setPosition(500, 500);
	testButton->setDimension(300, 100);
	auto testLevel = new Level("1");
	auto testAoeTower1 = new AoeTower("cannon", coords, testLevel);
	coords.x = 200;
	coords.y = 200;
	auto testHomingTower = new HomingTower("archer", coords, testLevel);
	auto testMenu = new Menu(testLevel);

	coords.x = 100;
	coords.y = 100;
	auto testIndustrialBuilding = new IndustrialBuilding("lumberjack", coords, testLevel);

	for (auto i = 0; i < 300000; i++)
	{
		//SDL_Delay(100);
		mAll_enemies.clear();
		testLevel->update();
		//add all enemies: for every wave every monstergroup in the level
		for(auto n = 0; n < testLevel->get_waves_count(); n++)
		{
			for(auto m = 0; m < testLevel->get_waves()->at(n).get_monster_group_count(); m++)
			{
				add_enemies(testLevel->get_waves()->at(n).get_monster_groups()->at(m).get_monsters());
			}
		}
		testAoeTower1->update(mAll_enemies);
		testHomingTower->update(mAll_enemies);
		
		SDL_RenderClear(gRenderer);
		testMap->render();
		testLevel->render();
		testAoeTower1->render();
		testHomingTower->render();
		testMenu->render();
		testButton->render();
		testIndustrialBuilding->render();
		SDL_RenderPresent(gRenderer);
		if (i % 60 == 0)
		{
			printf("Second...\n");
			testIndustrialBuilding->update();
		}
		if (testLevel->is_dead())
		{
			std::cout << "all enemies dead" << std::endl;
			SDL_Delay(10000);
			break;
		}

		if (testLevel->no_lives()) 
		{
			std::cout << "no lives" << std::endl;
			SDL_Delay(10000);
			break;
		}
	}
	delete testAoeTower1;
	delete testHomingTower;
	delete testMenu;
	delete testLevel;
	delete testMap;
	delete testButton;
	delete testIndustrialBuilding;
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

