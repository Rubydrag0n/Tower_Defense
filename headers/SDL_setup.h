#pragma once

#include <SDL.h>
#include "Textures.h"
#include <SDL_ttf.h>

const int LOGICAL_SCREEN_WIDTH = 1920;
const int LOGICAL_SCREEN_HEIGHT = 1080;
const int TILE_HEIGHT = 32;
const int TILE_WIDTH = 32;

const int MENU_ITEM_WIDTH = 70;
const int MENU_ITEM_HEIGHT = 70;

const int MATRIX_HEIGHT = 32;
const int MATRIX_WIDTH = 40;

extern int* gActual_screen_width;
extern int* gActual_screen_height;

extern unsigned* gFrame_rate;
extern unsigned* gTicks_per_frame;

extern SDL_Window* gWindow;
extern SDL_Renderer* gRenderer;
extern Textures* gTextures;
extern TTF_Font* gFont;

SDL_Texture* load_texture(const std::string& path);	//loads an image from the specified path

bool init_graphics();							//initializes the Graphics, generates the window etc.