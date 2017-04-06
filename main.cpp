//Using SDL and standard IO
#include <stdio.h>
//#include "game.h"
#include "SDL_setup.h"
#include "Map.h"
#include "Unit.h"

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

		SDL_Point p;
		p.x = 1750;
		p.y = 450;
		SDL_Point p1;
		p1.x = 1550;
		p1.y = 550;
		SDL_Point p2;
		p2.x = 1050;
		p2.y = 850;
		SDL_Point p3;
		p3.x = 150;
		p3.y = 250;
		SDL_Point p4;
		p4.x = 450;
		p4.y = 950;
		SDL_Point p5;
		p5.x = 550;
		p5.y = 750;
		auto testUnit = new Unit(p, "resources/LinkMonster.cfg");
		auto testUnit1 = new Unit(p1, "resources/LinkMonster.cfg");
		auto testUnit2 = new Unit(p2, "resources/LinkMonster.cfg");
		auto testUnit3 = new Unit(p3, "resources/LinkMonster.cfg");
		auto testUnit4 = new Unit(p4, "resources/LinkMonster.cfg");
		auto testUnit5 = new Unit(p5, "resources/LinkMonster.cfg");
		for (auto i = 0; i < 1000; i++)
		{
			//SDL_Delay(400);
			SDL_RenderClear(gRenderer);
			test->render();
			testUnit->render();
			testUnit1->render();
			testUnit2->render();
			testUnit3->render();
			testUnit4->render();
			testUnit5->render();
			SDL_RenderPresent(gRenderer);
		}
		delete testUnit;
		delete testUnit1;
		delete testUnit2;
		delete testUnit3;
		delete testUnit4;
		delete testUnit5;
		delete test;
	}



	return 0;
}
