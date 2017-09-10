#pragma once
#include "Enemy.h"
#include "CoordinatesInDouble.h"

class Tower;

class Shot
{
public:
	Shot(Tower* tower, Enemy *enemy_to_shot);
	~Shot();
	
	void render();
	bool follow();

	SDL_Point get_coords() const;
	void set_coords(SDL_Point coords);
	Enemy *get_enemy_to_shoot();

private:
	SDL_Point mCoords;
	CoordinatesInDouble mCoords_in_double;
	SDL_Texture *mSprite;
	SDL_Rect mSprite_dimensions;
	double mProjectile_speed;
	Enemy *mEnemy_to_shoot;

};

