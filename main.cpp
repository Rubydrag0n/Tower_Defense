//Using SDL and standard IO
#include <stdio.h>
//#include "game.h"
#include "SDL_setup.h"
#include "Map.h"
#include "Unit.h"
#include "MonsterGroup.h"
#include "Window.h"

int main( int argc, char* args[] )
{
	if (!initGraphics())
	{
		printf("Failed to initialize Graphics!\n");
	}
	else
	{
		auto test = new Map("level/Level1.FMP");

		const int winc = 10;

		SDL_Rect d[winc];
		for (auto i = 0; i < winc; i++) {
			d[i].x = rand() % 300 + 50;
			d[i].y = rand() % 300 + 50;
			d[i].w = rand() % 1000 + 50;
			d[i].h = rand() % 600 + 50;
		}
		Window *windows[winc];
		for (auto i = 0; i < winc; i++)
		{
			windows[i] = new Window(d[i]);
		}

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

			for (auto j = 0; j < winc; j++)
			{
				d[j].x += rand() % 9 - 4;
				d[j].y += rand() % 9 - 4;
				d[j].w += rand() % 9 - 4;
				if (d[j].w < 150) d[j].w = 150;
				d[j].h += rand() % 9 - 4;
				if (d[j].h < 150) d[j].h = 150;

				windows[j]->set_dim(d[j]);

				windows[j]->render();
			}
			SDL_RenderPresent(gRenderer);
			if (i % 60 == 0) printf("Second...\n");
			if (testMonsterGroup->isDead()) break;
		}
		delete testMonsterGroup;
		delete test;
		for (auto i = 0; i < winc; i++)
		{
			delete windows[i];
		}
	}
	
	SDL_Delay(1000);

	return 0;
}
