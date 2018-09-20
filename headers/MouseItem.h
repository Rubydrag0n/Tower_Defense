#pragma once
#include "Clickable.h"
#include "LTexture.h"
#include "Resources.h"


class Level;

class MouseItem : public Clickable
{
public:
	MouseItem(std::string name_of_object, LTexture* sprite, Level* level, Resources construction_costs);
	~MouseItem();

	void set_name_of_object(std::string name);
	std::string get_name_of_object();

	void on_click(int mouse_x, int mouse_y) override;
	void on_mouse_over(int mouse_x, int mouse_y) override;
	void on_right_click(int mouse_x, int mouse_y) override;
	void on_middle_click(int mouse_x, int mouse_y) override;

	void render(SDL_Point mouse_position);


private:
	std::string mName_of_object;
	LTexture* mSprite;
	Level* mLevel;
	Resources mConstruction_costs;
};
