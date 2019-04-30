#pragma once
#include <SDL.h>
#include "Window.h"
#include "Text.h"


class Unit;

class UnitWindow : public Window
{
public:
	UnitWindow(SDL_Rect dim, Unit* unit);
	~UnitWindow();

	void render() override;

private:
	Text** mDefense_names;
	Text** mDefense_values;
	Text* mMove_speed_name;
	Text* mMove_speed_value;

	Unit* mUnit;
};
