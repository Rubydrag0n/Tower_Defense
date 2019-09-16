#include "OnHitShot.h"
#include "Tower.h"

OnHitShot::OnHitShot(Tower* tower, SDL_Point location_to_shoot) : Shot(tower)
{
	auto const t_x = float(tower->get_coords().x + tower->get_dimensions().w / 2);
	auto const t_y = float(tower->get_coords().y + tower->get_dimensions().h / 2);
	auto const l_x = float(location_to_shoot.x);
	auto const l_y = float(location_to_shoot.y);
	auto const m = (t_y - l_y) / (t_x - l_x);
	auto const n = t_y - m * t_x;
	
	float y = 0;
	float x = 0;
	//enemy is right of the tower
	if(l_x > t_x)
	{
		y = m * 1280 + n;
		if(y >= 0 && y <= 1024)
		{
			location_to_shoot.x = 1280;
			location_to_shoot.y = y;
		}
		else if(l_y > t_y)
		{
			x = (1024 - n) / m;
			if(x <= 1280 && x >= 0)
			{
				location_to_shoot.x = x;
				location_to_shoot.y = 1024;
			}
		}
		else
		{
			x = (0 - n) / m;
			location_to_shoot.x = x;
			location_to_shoot.y = 0;
		}
	}
	//enemy is left of the tower
	else
	{
		y = m * 0 + n;
		if (y >= 0 && y <= 1024)
		{
			location_to_shoot.x = 0;
			location_to_shoot.y = y;
		}
		else if (l_y > t_y)
		{
			x = float((1024 - n) / m);
			if (x <= 1280 && x >= 0)
			{
				location_to_shoot.x = x;
				location_to_shoot.y = 1024;
			}
		}
		else
		{
			x = (0 - n) / m;
			location_to_shoot.x = x;
			location_to_shoot.y = 0;
		}
	}
	mTarget = location_to_shoot;
}

void OnHitShot::on_tick()
{
	if (damaged_an_enemy()) delete this;
	if (Shot::follow(mTarget)) delete this;
}

bool OnHitShot::follow()
{
	return Shot::follow(mTarget);
}



