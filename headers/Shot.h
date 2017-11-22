#pragma once
#include "LTexture.h"
#include "CoordinatesInDouble.h"

class Tower;

class Shot
{
public:
	Shot(Tower* tower);
	~Shot();
	
	void render(SDL_Point target) const;
	bool follow(SDL_Point target);

	SDL_Point get_coords() const;
	void set_coords(SDL_Point coords);


protected:
	SDL_Point mCoords;
	CoordinatesInDouble mCoords_in_double;
	double mProjectile_speed;

private:
	LTexture *mSprite;
	SDL_Rect mSprite_dimensions;	
};

