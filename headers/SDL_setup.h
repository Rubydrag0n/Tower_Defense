#pragma once

#include <SDL.h>
#include <string>

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

extern SDL_Window* gWindow;
extern SDL_Renderer* gRenderer;

SDL_Texture* loadTexture(std::string path);			//loads a image from the specified path

bool initGraphics();								//initializes the Graphics, generates the window etc.