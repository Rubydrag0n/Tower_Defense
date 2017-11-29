#pragma once
#include "LTexture.h"
#include "CoordinatesInDouble.h"

class Tower;

class Shot
{
public:
	Shot(Tower* tower);
	~Shot();
	
	
	void render(SDL_Point target); //renders the shot
	void points_projectile_to_target(SDL_Rect* dest, SDL_Point* center, double* angle, SDL_Point target);
	bool follow(SDL_Point target); //follows the enemy and returns true when it hits

	SDL_Point get_coords() const;
	void set_coords(SDL_Point coords);


protected:
	SDL_Point mCoords;
	CoordinatesInDouble mCoords_in_double;
	double mProjectile_speed;

private:
	LTexture *mSprite; //Texture
	SDL_Rect mSprite_dimensions; //size of texture	
};

