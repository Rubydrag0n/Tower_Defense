#pragma once
#include "ButtonObject.h"
#include "MainMenuTab.h"

class LevelSelectMenu : MainMenuTab, ButtonObject
{
public:
	LevelSelectMenu(Game* game);

	void on_button_press(int button_id, Button* button) override;

private:
};
