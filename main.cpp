//memory leak detection
/*#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h>  
*/

//Using SDL and standard IO
#include "SDL_setup.h"
#include "Map.h"
#include "Game.h"
#include <stdio.h>
#include "ConfigFile.h"


int main( int argc, char* args[] )
{
	gConfig_file = new ConfigFile("config/game.cfg");
	if (!init_graphics())
	{
		printf("Failed to initialize Graphics!\n");
	}
	else
	{
		auto game = new Game();
		game->start_game();
		
		delete game;
	}
	
	SDL_Delay(1000);

	return 0;
}
