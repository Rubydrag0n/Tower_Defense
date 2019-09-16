#pragma once
#include "Shot.h"

//shot that flies to a static location and triggers when it reaches the location
class StaticTargetShot final : 
	public Shot
{
public:
	StaticTargetShot(Tower* tower, SDL_Point location_to_shoot);

	void on_tick() override;
	
	bool follow(); //shot follows to a location
};
