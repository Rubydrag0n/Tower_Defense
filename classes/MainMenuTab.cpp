#include "MainMenuTab.h"
#include "LayerHandler.h"
#include "Button.h"
#include "SDL_setup.h"

MainMenuTab::MainMenuTab(Game* game) : Renderable(OVERLAY), mBackground_sprite(nullptr), mGame(game)
{
}

void MainMenuTab::render()
{
	SDL_Rect dest{ 0, 0, LOGICAL_SCREEN_WIDTH, LOGICAL_SCREEN_HEIGHT };

	gLayer_handler->render_to_layer(mBackground_sprite, mRender_layer, nullptr, &dest);
}

void MainMenuTab::set_enabled(bool enabled)
{
	for(auto button : mButtons)
	{
		button->set_rendering_enabled(enabled);
		if (enabled) button->enable();
		else button->disable();
	}
	this->set_rendering_enabled(enabled);
}



