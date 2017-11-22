#pragma once
#include "Shot.h"
#include <vector>



class AoeShot : 
	public Shot
{
public:
	AoeShot(Tower* tower, SDL_Point location_to_shoot);
	~AoeShot();

	
	bool follow(); //shot follows to a location
	SDL_Point get_location_to_shoot() const;

private:
	SDL_Point mLocation_to_shoot;
	std::vector<AoeShot*> mShots;
};

