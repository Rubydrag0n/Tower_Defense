#include "Shot.h"
#include "Enemy.h"
#include "ConfigFile.h"
#include "SDL_setup.h"
#include "Tower.h"
#include <iostream>
#include <math.h>


Shot::Shot(std::string projectile_name, Tower* tower, double projectile_speed, Enemy *enemy_to_shot)
{
	ConfigFile cf("config/game.cfg");
	mSprite = gTextures->get_texture(cf.Value(projectile_name + "/sprite", "path"));
	mSprite_dimensions.w = cf.Value(projectile_name + "/sprite", "image_width");
	mSprite_dimensions.h = cf.Value(projectile_name + "/sprite", "image_height");
	mCoords = tower->get_coords();
	mCoords_in_double.x = mCoords.x;
	mCoords_in_double.y = mCoords.y;
	mProjectile_speed = projectile_speed;
	mEnemy_to_shoot = enemy_to_shot;
}

Shot::~Shot()
{
}

void Shot::render()
{
	double angle_in_rad = 0;
	double x_d = mEnemy_to_shoot->get_position().x - mCoords.x;
	double y_d = mEnemy_to_shoot->get_position().y - mCoords.y;
	auto dist_to_enemy = x_d * x_d + y_d * y_d;
	dist_to_enemy = sqrt(dist_to_enemy);
	if(x_d < 0)
	{
		angle_in_rad = asin(-(y_d / dist_to_enemy));
	}
	else
	{
		if (y_d < 0)
		{
			angle_in_rad = asin(x_d / dist_to_enemy) + (M_PI / 2);
		}
		else
		{
			angle_in_rad = asin(-(x_d / dist_to_enemy)) - (M_PI / 2);
		}
	}
	double angle_in_deg = angle_in_rad / M_PI * 180;
	SDL_Rect dest;
	SDL_Point center;
	SDL_RendererFlip flip = SDL_FLIP_NONE;
	center.x = mSprite_dimensions.w / 2;
	center.y = mSprite_dimensions.h / 2;
	dest.x = mCoords.x - center.x;
	dest.y = mCoords.y - center.y / 2;
	dest.w = mSprite_dimensions.w;
	dest.h = mSprite_dimensions.h;
	SDL_RenderCopyEx(gRenderer, mSprite, &mSprite_dimensions, &dest, angle_in_deg, &center, flip);
}

//follows the enemy and returns true when it hits
bool Shot::follow()
{
	auto travel_dist = mProjectile_speed / 60.0;
	double x_d = mEnemy_to_shoot->get_position().x - mCoords.x;
	double x_d_abs = sqrt(x_d * x_d); //take the absolute value for further calculations
	double y_d = mEnemy_to_shoot->get_position().y - mCoords.y;
	double y_d_abs = sqrt(y_d * y_d); //same as above
	auto dist_to_enemy = x_d * x_d + y_d * y_d;
	dist_to_enemy = sqrt(dist_to_enemy);

	if(dist_to_enemy < travel_dist)
	{
		mCoords.x = mEnemy_to_shoot->get_position().x;
		mCoords.y = mEnemy_to_shoot->get_position().y;
		return true;
	}
	mCoords_in_double.x += travel_dist * (x_d / (x_d_abs + y_d_abs));
	mCoords_in_double.y += travel_dist * (y_d / (x_d_abs + y_d_abs));
	mCoords.x = mCoords_in_double.x;
	mCoords.y = mCoords_in_double.y;

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

Enemy *Shot::get_enemy_to_shoot()
{
	return mEnemy_to_shoot;
}
