//Using SDL and standard IO
#include <stdio.h>
//#include "game.h"
#include "SDL_setup.h"
#include "Map.h"
#include "Unit.h"
#include "MonsterGroup.h"
#include "Tower.h"

int main( int argc, char* args[] )
{
	if (!initGraphics())
	{
		printf("Failed to initialize Graphics!\n");
	}
	else
	{
		auto test = new Map("level/Level1.FMP");

		SDL_RenderClear(gRenderer);

		test->render();
		
		SDL_RenderPresent(gRenderer);
		SDL_Point coords;
		coords.x = 100;
		coords.y = 100;
		auto testTower = new Tower("tower1", coords);
		auto testMonsterGroup = new MonsterGroup("monster1", "level1", 0, 100, 10);
		for (auto i = 0; i < 3000; i++)
		{
			//SDL_Delay(400);
			testMonsterGroup->update();
			SDL_RenderClear(gRenderer);
			test->render();
			testMonsterGroup->render();
			testTower->render();
			SDL_RenderPresent(gRenderer);
			if (i % 60 == 0) printf("Second...\n");
			if (testMonsterGroup->isDead()) break;
		}
		delete testMonsterGroup;
		delete test;
	}
	
	SDL_Delay(1000);

	return 0;
}
