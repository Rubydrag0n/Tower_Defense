#include "Carriage.h"
#include "Entity.h"
#include "Unit.h"

Carriage::Carriage(std::string unit_name, Building* source, Building* drain) : Unit{ unit_name }
{
	this->mSource = source;
	this->mDrain = drain;
	this->mCapacity = new Resources();
	this->mCurrent_resources = new Resources();

	this->mCurrent_activity = ACTIVITY::GETTING;
}

Carriage::~Carriage()
{

}

void Carriage::render()
{
	Unit::render();
}

void Carriage::on_tick()
{
	this->move();
}

void Carriage::set_source(Building* b)
{
	this->mSource = b;
}

void Carriage::set_drain(Building* b)
{
	this->mDrain = b;
}

Building* Carriage::get_source()
{
	return this->mSource;
}

Building* Carriage::get_drain()
{
	return this->mDrain;
}

bool Carriage::move_towards(Building* target_building)
{
	double move_dist = mMove_speed / 60.0;
	auto target = target_building->get_coords();

	double x_d = target.x - mPosition.x;
	auto x_d_abs = abs(x_d); //take the absolute value for further calculations
	double y_d = target.y - mPosition.y;
	auto y_d_abs = abs(y_d); //same as above

	auto dist_to_enemy = sqrt(x_d * x_d + y_d * y_d);

	if (move_dist > dist_to_enemy)
	{
		//if close enough deliver resources and get back
		this->mPosition.x = target_building->get_coords().x;
		this->mPosition.y = target_building->get_coords().y;
		return true;
	}

	mPosition.x += move_dist * (x_d / (x_d_abs + y_d_abs));
	mPosition.y += move_dist * (y_d / (x_d_abs + y_d_abs));

	return false;
}

void Carriage::move()
{
	switch (this->mCurrent_activity)
	{
	case GETTING:
		//move towards source building
		if (this->mSource != nullptr)
		{
			if (this->move_towards(mSource))
			{
				this->mSource->transfer_resources_out(this->mCurrent_resources);
				this->mCurrent_activity = DELIVERING;
			}
		}
		break;
	case DELIVERING:
		if (this->mDrain != nullptr)
		{
			if (this->move_towards(mDrain))
			{
				this->mDrain->transfer_resources_in(this->mCurrent_resources);
				this->mCurrent_activity = GETTING;
			}
		}
		break;
	default:
		break;
	}
}
