#include "MainMenu.h"
#include "SDL_setup.h"
#include "ConfigFile.h"
#include "Game.h"
#include "LayerHandler.h"

MainMenu::MainMenu(Game* game) : Renderable(OVERLAY)
{
	const auto sprite_section = "MainMenu/sprite";

	mBackground_sprite = gTextures->get_texture(gConfig_file->value(sprite_section, "background"));
	mTitle_sprite = gTextures->get_texture(gConfig_file->value(sprite_section, "title"));
	mMain_menu_sprite = gTextures->get_texture(gConfig_file->value(sprite_section, "mainmenu"));

	SDL_Rect dim;
	dim.w = gConfig_file->value("button/play", "clip_width");
	dim.h = gConfig_file->value("button/play", "clip_height");
	dim.x = LOGICAL_SCREEN_WIDTH / 2 - dim.w;
	dim.y = 300;

	mPlay_button = new Button("play", dim, this, OVERLAY, OVERLAY, PLAY);

	dim.w = gConfig_file->value("button/quit", "clip_width");
	dim.h = gConfig_file->value("button/quit", "clip_height");
	dim.y = 500;
	mQuit_button = new Button("quit", dim, this, OVERLAY, OVERLAY, QUIT);

	dim.w = gConfig_file->value("button/settings", "clip_width");
	dim.h = gConfig_file->value("button/settings", "clip_height");
	dim.y = 700;
	mSettings_button = new Button("settings", dim, this, OVERLAY, OVERLAY, SETTINGS);

	dim.w = gConfig_file->value("button/stats", "clip_width");
	dim.h = gConfig_file->value("button/stats", "clip_height");
	dim.y = 900;
	mStats_button = new Button("stats", dim, this, OVERLAY, OVERLAY, STATS);

	this->set_enabled(true);

	mGame = game;
}

void MainMenu::render()
{
	SDL_Rect dest;

	dest.x = 0;
	dest.y = 0;
	dest.w = 1920;
	dest.h = 1080;
	
	gLayer_handler->render_to_layer(mBackground_sprite, mRender_layer, nullptr, &dest);
}

void MainMenu::set_enabled(const bool enabled)
{
	mPlay_button->set_rendering_enabled(enabled);
	mQuit_button->set_rendering_enabled(enabled);
	mSettings_button->set_rendering_enabled(enabled);
	mStats_button->set_rendering_enabled(enabled);

	this->set_rendering_enabled(enabled);

	if (enabled) {
		mPlay_button->enable();
		mQuit_button->enable();
		mSettings_button->enable();
		mStats_button->enable();
	} else
	{
		mPlay_button->disable();
		mQuit_button->disable();
		mSettings_button->disable();
		mStats_button->disable();
	}
}

void MainMenu::on_button_press(int button_id, Button* button)
{
	switch(button_id)
	{
	case int(PLAY):
		mState = STATE::LEVEL_SELECT;
		this->set_enabled(false);

		mGame->load_level(1);
		mGame->set_state(Game::STATE::PLAYING);

		break;

	case int(QUIT):
		mState = STATE::QUIT;
		mGame->set_state(Game::STATE::EXITING);
		break;

	case int(SETTINGS):
		mState = STATE::SETTINGS_MENU;
		break;

	case int(STATS):
		mState = STATE::STATS;
		break;

	case int(BACK) :
		this->set_enabled(true);

		mState = STATE::MAIN_MENU;
		break;

	default:
		printf("This button doesn't exist?!\n\r");
		break;
	}
}
