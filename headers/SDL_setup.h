#pragma once

#include <SDL.h>
#include "Textures.h"
#include <SDL_ttf.h>

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

extern SDL_Window* gWindow;
extern SDL_Renderer* gRenderer;
extern Textures* gTextures;
extern TTF_Font* gFont;

SDL_Texture* load_texture(std::string path);			//loads a image from the specified path

bool init_graphics();								//initializes the Graphics, generates the window etc.