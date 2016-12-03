#ifndef SDL_SETUP_H
#define SDL_SETUP_H

#include <SDL.h>
#include <string>

SDL_Texture* loadTexture(std::string path);			//loads a image from the specified path

bool initGraphics();								//initializes the Graphics, generates the window etc.



#endif