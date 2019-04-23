#pragma once
#include "Button.h"

class ShowMoreButton : public Button
{
public:
	ShowMoreButton(const std::string& button_name, SDL_Rect dim, ButtonObject* obj, Renderable* texture_to_render_on, LAYERS click_layer, LAYERS render_layers, int button_id);
	~ShowMoreButton();

private:
};