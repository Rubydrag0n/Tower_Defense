#include "Shot.h"
#include "Enemy.h"
#include "ConfigFile.h"
#include "SDL_setup.h"
#include "Tower.h"
#include <iostream>


Shot::Shot(std::string projectile_name, Tower tower, double projectile_speed, Enemy *enemy_to_shot)
{
	ConfigFile cf("config/game.cfg");
	mSprite =gTextures->get_texture(cf.Value(projectile_name + "/sprite", "path"));
	mSprite_dimensions.w = cf.Value(projectile_name + "/sprite", "image_width");
	mSprite_dimensions.h = cf.Value(projectile_name + "/sprite", "image_height");
	mCoords = tower.get_coords();
	mProjectile_speed = projectile_speed;
	mEnemy_to_shot = enemy_to_shot;
}

Shot::~Shot()
{
}

void Shot::render()
{
	SDL_Rect dest;
	dest.x = mCoords.x - mSprite_dimensions.w / 2;
	dest.y = mCoords.y - mSprite_dimensions.h / 2;
	dest.w = mSprite_dimensions.w;
	dest.h = mSprite_dimensions.h;
	SDL_RenderCopy(gRenderer, mSprite, &mSprite_dimensions, &dest);
}

//follows the enemy and returns true when it hits
bool Shot::follow()
{
	std::cout << "follow" << std::endl;
	auto travel_dist = mProjectile_speed / 60.0;
	double x_div = mEnemy_to_shot->get_position().x - mCoords.x;
	double x_div_abs = sqrt(x_div * x_div); //take the absolute value for further calculations
	double y_div = mEnemy_to_shot->get_position().y - mCoords.y;
	double y_div_abs = sqrt(y_div * y_div); //same as above
	auto dist_to_enemy = x_div * x_div + y_div * y_div;
	dist_to_enemy = sqrt(dist_to_enemy);

	if(dist_to_enemy < travel_dist)
	{
		mCoords.x = mEnemy_to_shot->get_position().x;
		mCoords.y = mEnemy_to_shot->get_position().y;
		std::cout << "hit" << std::endl;
		return true;
	}
	std::cout << "travel_dist: " << travel_dist << std::endl;
	std::cout << "x_div " << x_div << std::endl;
	std::cout << "y_div " << y_div << std::endl;
	mCoords.x += travel_dist * (x_div / (x_div_abs + y_div_abs));
	mCoords.y += travel_dist * (y_div / (x_div_abs + y_div_abs));
	std::cout << "x: " << mCoords.x << std::endl;
	std::cout << "y: " << mCoords.y << std::endl;
	std::cout << "x_enemy: " << mEnemy_to_shot->get_position().x << std::endl;
	std::cout << "y_enemy: " << mEnemy_to_shot->get_position().y << std::endl;

	return false;
}

SDL_Point Shot::get_coords() const
{
	return mCoords;
}

void Shot::set_coords(SDL_Point coords)
{
	mCoords = coords;
}

Enemy Shot::get_enemy_to_shot()
{
	return *mEnemy_to_shot;
}
