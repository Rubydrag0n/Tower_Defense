#pragma once

#include <SDL.h>
#include "Textures.h"
#include <SDL_ttf.h>

const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;
const int TILE_HEIGHT = 64;
const int TILE_WIDTH = 64;

extern SDL_Window* gWindow;
extern SDL_Renderer* gRenderer;
extern Textures* gTextures;
extern TTF_Font* gFont;

SDL_Texture* load_texture(std::string path);	//loads a image from the specified path

bool init_graphics();							//initializes the Graphics, generates the window etc.