#pragma once
#include "LTexture.h"
#include "CoordinatesInDouble.h"
#include "Damage.h"
#include "Entity.h"

class Tower;

class Shot
	: public Entity
{
public:
	Shot(Tower* tower);
	~Shot();
	
	void render() override; //renders the shot
	virtual void on_tick() = 0;
	void points_projectile_to_target(SDL_Rect* dest, SDL_Point* center, double* angle) const;
	bool follow(SDL_Point target); //follows the enemy and returns true when it hits

	ENTITYTYPE get_type() override;

	SDL_Point get_coords() const;
	void set_coords(SDL_Point coords);

	void set_target(SDL_Point target);

protected:
	SDL_Point mCoords;
	CoordinatesInDouble mCoords_in_double;
	double mProjectile_speed;

	//the damage this shot will do
	Damage mDamage;

private:
	LTexture *mSprite; //Texture
	SDL_Rect mSprite_dimensions; //size of texture	
	SDL_Point mTarget; //the point the shot is targeting
};

