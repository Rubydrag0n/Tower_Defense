#include "MainMenu.h"
#include "SDL_setup.h"
#include "ConfigFile.h"
#include "LayerHandler.h"

MainMenu::MainMenu()
{
	mSprite_path = std::string(gConfig_file->value("MainMenu/sprite", "path"));
	mSprite = gTextures->get_texture(mSprite_path);
}

MainMenu::~MainMenu()
{
	
}


void MainMenu::render()
{
	SDL_Rect dest;

	dest.x = 0;
	dest.y = 0;
	dest.w = 1920;
	dest.h = 1080;
	gLayer_handler->render_to_layer(mSprite, LAYERS::WINDOWS, nullptr, &dest);
}
