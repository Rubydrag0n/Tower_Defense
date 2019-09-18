#include <SDL.h>
#include "EntityHandler.h"
#include "StaticTargetShot.h"
#include "Particles.h"
#include "Tower.h"

StaticTargetShot::StaticTargetShot(Tower* tower, Enemy* enemy) : Shot(tower)
{
	//target is set once the shot exists, it wont change
	mTarget = enemy->get_position();
	if(tower->get_predicts())
	{
		Vector enemies_position;
		enemies_position.x = mTarget.x;
		enemies_position.y = mTarget.y;
		Vector towers_position;
		towers_position.x = tower->get_coords().x - tower->get_dimensions().w / 2;
		towers_position.y = tower->get_coords().y - tower->get_dimensions().h / 2;
		auto const v_t = tower->get_projectile_speed();
		Vector v_e; //vector for speed and direction of enemy
		v_e.x = enemy->get_checkpoints().at(0).x - enemies_position.x;
		v_e.y = enemy->get_checkpoints().at(0).y - enemies_position.y;
		auto const v_e1 = v_e / v_e.get_amount(); //unit vector from v_e
		v_e = v_e1 * enemy->get_move_speed();

		auto m_e = v_e.y / v_e.x; //increase
		// can not divide through zero
		// for the moment set some big numbers for m if this happens
		if(v_e.x == 0)
		{
			if (v_e.y < 0) m_e = 1000000000000;
			else m_e = -1000000000000;
		}
		auto n_e = enemies_position.y - m_e * enemies_position.x;

		auto m_r = -1 / m_e;
		if (m_e == 0) m_r = -1000000000000000;
		auto const n_r = towers_position.y - m_r * towers_position.x;
		auto const s_x = (n_r - n_e) / (m_e - m_r); //intersection points between the movement path of the enemy and the line that goes through the tower and intersects the path of the enemy orthogonal
		auto const s_y = m_e * s_x + n_e;
		Vector r; //vector that points with a right angel on v_e
		r.x = s_x-towers_position.x;
		r.y = s_y-towers_position.y;
		auto const temp = v_e.get_amount() * v_e.get_amount();
		auto const a = v_t * v_t - temp;
		auto const b = enemies_position * (- 2.0) * v_t * v_t
		+ towers_position * 2 * temp + r * 2 * temp;
		auto const c = enemies_position * enemies_position * v_t * v_t
		- (towers_position * towers_position + towers_position * r * 2 + r * r) * temp - v_e * r.get_amount();
		auto const q = c / a;
		auto const p = b / (2 * a);
		auto const enemies_predicted_position1 = p * -1 + (p * p - q).root();
		auto const enemies_predicted_position2 = p * -1 - (p * p - q).root();
		if(enemies_predicted_position1.x < 0 || enemies_predicted_position1.y < 0)
		{
			mTarget.x = enemies_predicted_position2.x;
			mTarget.y = enemies_predicted_position2.y;
		}
		else
		{
			mTarget.x = enemies_predicted_position1.x;
			mTarget.y = enemies_predicted_position1.y;
		}

	}
}

void StaticTargetShot::on_tick()
{
	if (follow())
	{
		damaged_an_enemy();
		delete this;
	}
}

bool StaticTargetShot::follow()
{
	const auto target_reached = Shot::follow(mTarget);

	if (target_reached) {
		Vector coordinates, direction;
		coordinates.x = mTarget.x + (rand() % 7 - 3);
		coordinates.y = mTarget.y + (rand() % 7 - 3);
		direction.x = (rand() % 31 - 15) / 40.0;
		direction.y = (rand() % 31 - 15) / 40.0;
		const auto rotation_speed = (rand() % 31 - 15) / 40.f;
		const auto rotation = float(rand() % 360);
		new Particle("explosion", coordinates, direction, rotation, rotation_speed);
	}
	return target_reached;
}
