//Here comes the definiton of the game class
#include "Game.h"
#include "SDL_setup.h"
#include "MonsterGroup.h"
#include "Map.h"
#include <stdio.h>
#include "Tower.h"
#include <iostream>

Game::Game()
{
}

Game::~Game()
{
}

void Game::start_game()
{
	auto test = new Map("level/Level1.FMP");

	SDL_RenderClear(gRenderer);

	test->render();

	SDL_RenderPresent(gRenderer);
	SDL_Point coords;
	coords.x = 600;
	coords.y = 600;
	auto testTower1 = new Tower("tower1", coords);
	coords.y = 300;
	auto testTower = new Tower("tower1", coords);
	//add_tower(*testTower);
	auto testMonsterGroup = new MonsterGroup("monster1", "level1", 0, 100, 10);
	for (auto i = 0; i < 3000; i++)
	{
		//SDL_Delay(100);
		mAll_enemies.clear();
		testMonsterGroup->update();
		add_enemies(testMonsterGroup->get_monsters());
		testTower->update(mAll_enemies);
		testTower1->update(mAll_enemies);
		SDL_RenderClear(gRenderer);
		test->render();
		testMonsterGroup->render();
		testTower->render();
		testTower1->render();
		SDL_RenderPresent(gRenderer);
		if (i % 60 == 0) printf("Second...\n");
		if (testMonsterGroup->isDead()) break;
	}
	delete testMonsterGroup;
	delete test;
	delete testTower;
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

