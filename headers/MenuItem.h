#pragma once
#include "Clickable.h"
#include "Building.h"


enum class CLICKSTATE
{
	UNCLICKED,
	LEFTCLICKED,
	COUNT
};
class MenuItem :
	public Clickable
{
public:
	MenuItem(std::string name_of_object, Level *level);
	~MenuItem();

	void render(SDL_Point mouse_position);

	void on_click(int mouse_x, int mouse_y) override;
	void on_mouse_over(int mouse_x, int mouse_y) override;
	void on_right_click(int mouse_x, int mouse_y) override;
	void on_middle_click(int mouse_x, int mouse_y) override;

	CLICKSTATE getClickstate();
	LTexture* get_sprite();
	SDL_Point get_coords();


private:
	SDL_Point mCoords; //coordinates, where the item is placed in the menu
	LTexture* mSprite;
	std::string mName_of_object; // for example the tower name
	Level *mLevel;
	CLICKSTATE mClickstate;
};
