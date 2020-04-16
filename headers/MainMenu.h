#pragma once
#include "LTexture.h"
#include "Button.h"
#include "MainMenuTab.h"

class Game;

class MainMenu : MainMenuTab, ButtonObject
{
public:
	explicit MainMenu(Game* game);

	void on_button_press(int button_id, Button* button) override;

private:
	LTexture *mTitle_sprite;
	LTexture *mMain_menu_sprite;
};
