#pragma once
#include "Enemy.h"

class Tower;

class Shot
{
public:
	Shot(std::string shot_name, Tower tower, double projectile_speed, Enemy *enemy_to_shot);
	~Shot();
	
	void render();
	bool follow();

	SDL_Point get_coords() const;
	void set_coords(SDL_Point coords);
	Enemy get_enemy_to_shot();

private:
	SDL_Point mCoords;
	SDL_Texture *mSprite;
	SDL_Rect mSprite_dimensions;
	double mProjectile_speed;
	Enemy *mEnemy_to_shot;

};

