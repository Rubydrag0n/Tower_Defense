//Using SDL and standard IO
#include "SDL_setup.h"
#include "Map.h"
#include "Game.h"
#include <stdio.h>


int main( int argc, char* args[] )
{
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
