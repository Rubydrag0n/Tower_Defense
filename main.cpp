//Using SDL and standard IO
#include <stdio.h>
//#include "game.h"
#include "SDL_setup.h"
#include "Map.h"
#include "Unit.h"
#include "Enemy.h"

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

		auto testUnit = new Enemy("monster1", "level1", 0);
		for (auto i = 0; i < 10000; i++)
		{
			//SDL_Delay(400);
			testUnit->move();
			SDL_RenderClear(gRenderer);
			test->render();
			testUnit->render();
			SDL_RenderPresent(gRenderer);
			if (testUnit->isDead())
			{
				break;
			}
			if (i % 60 == 0) printf("Second...\n");
		}
		delete testUnit;
		delete test;
	}
	
	SDL_Delay(1000);

	return 0;
}
