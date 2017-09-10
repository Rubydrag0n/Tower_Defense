//Using SDL and standard IO
#include <stdio.h>
//#include "game.h"
#include "SDL_setup.h"
#include "Map.h"
#include "Game.h"
#include <SDL_ttf.h>


int main( int argc, char* args[] )
{
	if (!initGraphics())
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
