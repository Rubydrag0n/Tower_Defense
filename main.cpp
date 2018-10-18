//memory leak detection
/*#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h>  
*/
#include <cstdlib>
#include <stdio.h>
#include <iostream>
#include <time.h>
#include "SDL_setup.h"
#include "Map.h"
#include "Game.h"
#include "ConfigFile.h"
#include "EntityHandler.h"
#include "MouseHandler.h"
#include "MainMenu.h"
#include <fstream>

int main( int argc, char* args[] )
{
	//TODO: Needs error handling
	gConfig_file = new ConfigFile("config/game.cfg");
	if (!init_graphics())
	{
		printf("Failed to initialize Graphics!\n");
	}
	else
	{
		//initialize randomness
		srand(time(NULL));

		auto game = new Game();
		game->start_game();
		
		delete game;
	}
	
	SDL_Delay(1000);
	return 0;
}
