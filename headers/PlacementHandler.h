#pragma once
#include "Clickable.h"
#include <string>

class PlacementHandler
	: public Clickable
{
public:
	PlacementHandler(Level* level);
	~PlacementHandler();

	void on_click(int mouse_x, int mouse_y) override;
	

	bool isHolding();

	void set_level(Level* level);
	void set_held_object(std::string object_name);

private:

	Level* mLevel;
	std::string mHeld_object;	//holds the name of the object to be placed. is empty if no item is held
};

extern PlacementHandler* gPlacement_handler;