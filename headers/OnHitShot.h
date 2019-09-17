#pragma once
#include <SDL.h>
#include "Shot.h"

class OnHitShot final : public Shot
{
public:
	OnHitShot(Tower* tower, SDL_Point location_to_shoot);

	void on_tick() override;

	bool damaged_an_enemy() override;

	bool follow();
};
