#pragma once
#include "LTexture.h"
#include "Button.h"

class Game;

class MainMenu : ButtonObject, Renderable
{
public:
	explicit MainMenu(Game* game);

	void render() override;

	void set_enabled(bool enabled);

	void on_button_press(int button_id, Button* button) override;

private:
	enum class STATE
	{
		MAIN_MENU,
		SETTINGS_MENU,
		LEVEL_SELECT,
		STATS,
		QUIT
	} mState;

	LTexture *mBackground_sprite;
	LTexture *mTitle_sprite;
	LTexture *mMain_menu_sprite;
	
	Button *mPlay_button;
	Button *mQuit_button;
	Button *mSettings_button;
	Button *mStats_button;

	Game* mGame;
};
