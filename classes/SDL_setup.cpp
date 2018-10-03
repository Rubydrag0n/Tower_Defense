#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <cstdio>
#include <string>
#include <ctime>
#include "SDL_setup.h"
#include "ConfigFile.h"
#include "Textures.h"

SDL_Renderer* gRenderer = nullptr;

SDL_Window* gWindow = nullptr;

Textures* gTextures = nullptr;

TTF_Font* gFont = nullptr;

int* ACTUAL_SCREEN_WIDTH = nullptr;
int* ACTUAL_SCREEN_HEIGHT = nullptr;

SDL_Texture* load_texture(std::string path)
{
	//The final texture
	SDL_Texture* new_texture = nullptr;

	//Load image at specified path
	auto loaded_surface = IMG_Load(path.c_str());
	if (loaded_surface == nullptr)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		//Create texture from surface pixels
		new_texture = SDL_CreateTextureFromSurface(gRenderer, loaded_surface);
		if (new_texture == nullptr)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loaded_surface);
	}

	return new_texture;
}

bool init_graphics()
{
	//Initialization flag
	auto success = true;

	//Initalize randomness
	srand(time(nullptr));

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		//Create window
		ConfigFile cf("config/game.cfg");
		int screen_width = cf.Value("video", "screen_width");
		int screen_height = cf.Value("video", "screen_height");

		std::string windowflags = cf.Value("video", "window_mode");
		Uint32 flags;
		if (windowflags == "fullscreen") flags = SDL_WINDOW_FULLSCREEN;
		else if (windowflags == "borderless") flags = SDL_WINDOW_BORDERLESS | SDL_WINDOW_FULLSCREEN;
		else if (windowflags == "windowed") flags = SDL_WINDOW_OPENGL;
		else flags = 0;
		gWindow = SDL_CreateWindow("TOWER DEFENSE", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_width, screen_height, flags);

		ACTUAL_SCREEN_WIDTH = new int();
		ACTUAL_SCREEN_HEIGHT = new int();
		SDL_GetWindowSize(gWindow, ACTUAL_SCREEN_WIDTH, ACTUAL_SCREEN_HEIGHT);

		if (gWindow == nullptr)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create renderer for window
			//SDL_RENDERER_PRESENTVSYNC locks framerate to the monitors' framerate

			//TODO: don't just use vsync with 60fps
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_PRESENTVSYNC);
			if (gRenderer == nullptr)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//set logical screen resolution
				SDL_RenderSetLogicalSize(gRenderer, 1920, 1080);
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);	//so alpha can be used

				//Initialize PNG loading
				int img_flags = IMG_INIT_PNG;
				if (!(IMG_Init(img_flags) & img_flags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}

				//Initialize SDL_ttf
				if (TTF_Init() == -1)
				{
					printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
					success = false;
				}

				gFont = TTF_OpenFont("resources/lazy.ttf", 28);

			}
		}
	}

	//initialize Textures Object
	gTextures = new Textures();

	return success;
}