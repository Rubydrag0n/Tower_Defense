//Using SDL and standard IO
#include <stdio.h>
//#include "game.h"
#include "SDL_setup.h"
#include "Map.h"
#include "Unit.h"
#include "MonsterGroup.h"

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

		auto testMonsterGroup = new MonsterGroup("monster1", "level1", 0, 70, 20);
		for (auto i = 0; i < 3500; i++)
		{
			//SDL_Delay(400);
			testMonsterGroup->update();
			SDL_RenderClear(gRenderer);
			test->render();
			testMonsterGroup->render();
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
