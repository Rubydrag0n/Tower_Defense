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

void Carriage::move()
{
	switch (this->mCurrent_activity)
	{
	case GETTING:
		//move towards source building
		if (this->mSource != nullptr)
		{
			double move_dist = mMove_speed*mMove_speed / 3600.0;
			if ( move_dist >
				abs(mPosition.x - mSource->get_coords().x) * abs(mPosition.y - mSource->get_coords().y))
			{
				//if close enough deliver resources and get back
				this->mPosition.x = mSource->get_coords().x;
				this->mPosition.y = mSource->get_coords().y;
				this->mSource->transfer_resources(this->mCurrent_resources);
				this->mCurrent_activity = DELIVERING;
			}
			else 
			{
				auto target = mSource->get_coords();
				//otherwise move
				//TODO: move on paths
				if (this->mPosition.x < mSource->get_coords().x) {
					this->mPosition.x = mSource->get_coords().x;
					this->mPosition.y = mSource->get_coords().y;
					break;
				}

				double x_d = target.x - mPosition.x;
				auto x_d_abs = sqrt(x_d * x_d); //take the absolute value for further calculations
				double y_d = target.y - mPosition.y;
				auto y_d_abs = sqrt(y_d * y_d); //same as above

				mPosition.x += move_dist * (x_d / (x_d_abs + y_d_abs));
				mPosition.y += move_dist * (y_d / (x_d_abs + y_d_abs));
				break;
			}
		}
		break;
	case DELIVERING:

		break;
	default:
		break;
	}
}
