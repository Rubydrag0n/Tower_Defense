#include "UnitWindow.h"
#include "Defense.h"
#include "Unit.h"

UnitWindow::UnitWindow(SDL_Rect dim, Unit* unit) : Window(dim, WINDOWS, WINDOWS), mUnit(unit)
{
	mDefense_names = new Text*[RESISTANCES_TOTAL];
	mDefense_values = new Text*[RESISTANCES_TOTAL];
	SDL_Color text_color = { 0,0,0,0 };

	SDL_Rect dest;
	dest.x = mDim.x + 20;
	dest.y = mDim.y + 20;
	dest.w = 0;
	dest.h = 0;
	mHealth_name = new Text("Health", dest, WINDOWCONTENT, text_color, this);
	mHealth_value = new Text(std::to_string((int)mUnit->get_defense()->get_full_health()), dest, WINDOWCONTENT, text_color, this);
	mHealth_value->add_x_dim(100);
	for(auto i = 0; i < RESISTANCES_TOTAL; i++)
	{
		dest.y += 20;
		mDefense_names[i] = new Text(Defense::get_name(RESISTANCES(i)), dest, WINDOWCONTENT, text_color, this);
		mDefense_values[i] = new Text(std::to_string(mUnit->get_defense()->get_display_defenses().get_resistance(RESISTANCES(i))), dest, WINDOWCONTENT, text_color, this);
		mDefense_values[i]->add_x_dim(100);
	}
	dest.y += 20;
	mMove_speed_name = new Text("Move Speed", dest, WINDOWCONTENT, text_color, this);
	mMove_speed_value = new Text(std::to_string((int)mUnit->get_move_speed()), dest, WINDOWCONTENT, text_color, this);
	mMove_speed_value->add_x_dim(100);
}

UnitWindow::~UnitWindow()
{
	for (auto i = 0; i < RESISTANCES_TOTAL; i++)
	{
		delete mDefense_names[i];
		delete mDefense_values[i];
	}
	delete[] mDefense_names;
	delete[] mDefense_values;
	delete mHealth_name;
	delete mHealth_value;
	delete mMove_speed_name;
	delete mMove_speed_value;
}

void UnitWindow::render()
{
	Window::render();
	for (auto i = 0; i < RESISTANCES_TOTAL; i++)
	{
		mDefense_values[i]->set_text(std::to_string(mUnit->get_defense()->get_display_defenses().get_resistance(RESISTANCES(i))));
	}
	mMove_speed_value->set_text(std::to_string((int)mUnit->get_move_speed()));
}
