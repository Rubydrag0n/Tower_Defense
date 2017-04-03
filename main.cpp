//Using SDL and standard IO
#include <stdio.h>
//#include "game.h"
#include "SDL_setup.h"
#include "SDL_config.h"
#include "Map.h"

int main( int argc, char* args[] )
{
	if (!initGraphics())
	{
		printf("Failed to initialize Graphics!\n");
	}
	else
	{
		auto test = new Map("level/Level1.FMP");
		SDL_Surface *surface = nullptr;

		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(gRenderer);
		
		surface = SDL_LoadBMP("resources/background.bmp");

		test->render(surface);

		auto texture = SDL_CreateTextureFromSurface(gRenderer, surface);
		SDL_FreeSurface(surface);

		SDL_RenderCopy(gRenderer, texture, nullptr, nullptr);
		SDL_RenderPresent(gRenderer);
	}

	system("Pause");

	return 0;
}
