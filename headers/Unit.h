#pragma once
#include <SDL.h>
#include <vector>
#include "Defense.h"
#include "LTexture.h"
#include "Entity.h"
#include "CoordinatesInDouble.h"
#include "Enums.h"
#include "Clickable.h"
#include "UnitWindow.h"


/*
 * A Unit
 * Can walk over the screen with animation
 * Has defensive stats and movement speed
 */
class Unit : public Entity, public Clickable
{
public:
	explicit Unit(const std::string& unit_name, LAYERS render_layer);
	~Unit();

	void render() override;

	Defense* get_defense();
	double get_move_speed();

protected:
	void update_animation_clip();
	double get_rotation_angle() const;


	void on_click(int mouse_x, int mouse_y) override;

	Defense* mDefense;
	double mMove_speed;
	
	CoordinatesInDouble mPosition;
	//Only up, right, down or left for the choosing of the right sprite
	DIRECTION mDirection;

	SDL_Point mCenter;
	int mAnimation_tick;
	int mAnimation_tick_count;
	int mTickcount_per_clip;
	LTexture *mSprite;
	std::vector<SDL_Rect> mClips;
	SDL_Rect mCurrent_clip;
	SDL_Rect mSprite_dimensions;

	UnitWindow* mUnit_window;
};
