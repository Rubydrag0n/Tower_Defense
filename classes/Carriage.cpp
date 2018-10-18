#include "Carriage.h"
#include "Entity.h"
#include "Unit.h"
#include "SDL_setup.h"

Carriage::Carriage(std::string unit_name, Level* level, Building* source, Building* drain) : Unit{ unit_name }
{
	this->mSource = source;
	this->mDrain = drain;
	this->mCapacity = new Resources();
	this->mCurrent_resources = new Resources();

	this->mCurrent_activity = ACTIVITY::GETTING_IDLE;

	this->mLevel = level;

	if (mSource != nullptr)
	{
		mPosition.x = mSource->get_coords().x;
		mPosition.y = mSource->get_coords().y;
	}
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

bool Carriage::move_towards(SDL_Point target)
{
	double move_dist = mMove_speed / 60.0;

	double x_d = target.x - mPosition.x;
	auto x_d_abs = abs(x_d); //take the absolute value for further calculations
	double y_d = target.y - mPosition.y;
	auto y_d_abs = abs(y_d); //same as above

	auto dist_to_enemy = sqrt(x_d * x_d + y_d * y_d);

	if (move_dist > dist_to_enemy)
	{
		this->mPosition.x = target.x;
		this->mPosition.y = target.y;
		return true;
	}

	mPosition.x += move_dist * (x_d / (x_d_abs + y_d_abs));
	mPosition.y += move_dist * (y_d / (x_d_abs + y_d_abs));

	return false;
}

void Carriage::move()
{
	int x_tile = this->mPosition.x / TILE_WIDTH;
	int y_tile = this->mPosition.y / TILE_HEIGHT;
	Building* here = mLevel->get_building_matrix(x_tile, y_tile);

	switch (this->mCurrent_activity)
	{
	case GETTING_IDLE:		
		if (update_checkpoints_to(here, mSource))
		{
			this->mCurrent_activity = GETTING;
		}
		break;
	case GETTING:
		if (mCheckpoints.size() == 0)
		{
			//carriage arrived
			this->mSource->transfer_resources_out(this->mCurrent_resources);
			this->mCurrent_activity = DELIVERING_IDLE;
			break;
		}

		if (this->move_towards(mCheckpoints[0]))
		{
			//delete first checkpoint
			mCheckpoints.erase(mCheckpoints.begin());
		}
		break;
	case DELIVERING_IDLE:
		if (update_checkpoints_to(here, mDrain))
		{
			this->mCurrent_activity = DELIVERING;
		}
		break;
	case DELIVERING:
		if (mCheckpoints.size() == 0)
		{
			//carriage arrived
			this->mDrain->transfer_resources_in(this->mCurrent_resources);
			this->mCurrent_activity = GETTING_IDLE;
			break;
		}
		if (this->move_towards(mCheckpoints[0]))
		{
			//delete first checkpoint
			mCheckpoints.erase(mCheckpoints.begin());
		}
		break;
	default:
		break;
	}
}

//if path exists returns true and sets the path into mCheckpoints, otherwise returns false
bool Carriage::update_checkpoints_to(Building * source, Building * target)
{
	//TODO: make this thing less terrible

	if (source == nullptr || target == nullptr) return false;

	//if already there
	if (source == target) return true;

	//breadth-first search because there isn't enough space for difficult searches
	//and we find the shortest path guranteed
	std::map<int, std::vector<std::pair<Building*, Building*>>> depth_search;
	//		depth, vector of buildings in depth with mapped precursor
	depth_search[0].emplace_back(std::pair<Building*, Building*>(target, nullptr));

	bool success = false;
	int distance = 0;

	for (int depth = 0; !success; depth++) {	//going "up" through the depths
		//break if theres nothing more to search
		if (depth_search[depth].size() == 0) break;
		for (auto it = depth_search[depth].begin(); it != depth_search[depth].end(); ++it) {	//iterating through each depth
			
			Building* current = it->first;
			
			for (int dir = 0; dir != BUILDINGDIRECTIONS_TOTAL; dir++) { //iterating through 
				Building* neighbour = current->get_neighbour(BUILDINGDIRECTION(dir));

				if (neighbour != nullptr) {
					bool already_contained = false;
					//search if this building exists already somewhere
					for (auto i = 0; i < depth + 1; i++) {
						for (auto it2 = depth_search[i].begin(); it2 != depth_search[i].end(); it2++) {
							if (it2->first == neighbour) already_contained = true;
						}
					}
					if (!already_contained) {
						//if this new building hasn't been visited
						depth_search[depth + 1].emplace_back(std::pair<Building*, Building*>(neighbour, it->first));
						if (neighbour == source)
						{
							success = true;
							distance = depth + 1;
						}
					}
				}
			}
		}
	}
	// target not found
	if (distance == 0) return false;

	//collect path from source to target
	Building* current = source;
	for (auto depth = distance; depth > 0; depth--)
	{
		for (auto it = depth_search[depth].begin(); it != depth_search[depth].end(); ++it) {
			if (it->first == current)
			{
				//add to checkpoints
				mCheckpoints.emplace_back(it->second->get_coords());
				current = it->second;
			}
		}
	}
	return true;
}
