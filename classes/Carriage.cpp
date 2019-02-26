#include "Carriage.h"
#include "Unit.h"
#include "SDL_setup.h"
#include <set>
#include "ConfigFile.h"

Carriage::Carriage(const std::string& unit_name, Level* level, Building* source, Building* drain) : Unit{ unit_name }, mSource(source), mDrain(drain), mCurrent_activity(GETTING_IDLE), mLevel(level)
{
	this->mCurrent_resources = new Resources();

	if (mSource != nullptr)
	{
		mPosition.x = mSource->get_coords().x;
		mPosition.y = mSource->get_coords().y;
	}

	auto section = unit_name + "/stats";
	Resources limit;
	limit.set_resources(gConfig_file->value_or_zero(section, "goldcapacity"),
			gConfig_file->value_or_zero(section, "woodcapacity"),
			gConfig_file->value_or_zero(section, "stonecapacity"),
			gConfig_file->value_or_zero(section, "ironcapacity"),
			gConfig_file->value_or_zero(section, "energycapacity"),
			gConfig_file->value_or_zero(section, "watercapacity"),
			gConfig_file->value_or_zero(section, "foodcapacity"));
	
	this->mCurrent_resources->set_limit(&limit);
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

Building* Carriage::get_source() const
{
	return this->mSource;
}

Building* Carriage::get_drain() const
{
	return this->mDrain;
}

bool Carriage::move_towards(const SDL_Point target)
{
	const auto move_dist = mMove_speed / 60.0;

	const auto x_d = target.x - mPosition.x;
	const auto x_d_abs = abs(x_d); //take the absolute value for further calculations
	const auto y_d = target.y - mPosition.y;
	const auto y_d_abs = abs(y_d); //same as above

	const auto distance_to_target = sqrt(x_d * x_d + y_d * y_d);

	if (move_dist > distance_to_target)
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
	const auto x_tile = static_cast<int>(this->mPosition.x / TILE_WIDTH);
	const auto y_tile = static_cast<int>(this->mPosition.y / TILE_HEIGHT);
	const auto here = mLevel->get_building_matrix(x_tile, y_tile);

	switch (this->mCurrent_activity)
	{
	case GETTING_IDLE:		
		if (update_checkpoints_to(here, mSource))
		{
			this->mCurrent_activity = GETTING;
		}
		break;
	case GETTING:
		if (mCheckpoints.empty())
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
		if (mCheckpoints.empty())
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
	if (source == nullptr || target == nullptr) return false;

	//if already there
	if (source == target) return true;

	//breadth-first search because there isn't enough space for difficult searches
	//and we find the shortest path guaranteed
	std::map<int, std::vector<std::pair<Building*, Building*>>> depth_search;
	//what buildings have already been visited
	std::set<Building*> visited;
	//		depth, vector of buildings in depth with mapped precursor
	depth_search[0].emplace_back(std::pair<Building*, Building*>(target, nullptr));
	visited.insert(target);

	auto success = false;
	auto distance = 0;

	for (auto depth = 0; !success; depth++) {	//going "up" through the depths
		//break if there's nothing more to search
		if (depth_search[depth].empty()) break;
		for (auto it = depth_search[depth].begin(); it != depth_search[depth].end(); ++it) {	//iterating through each depth

			auto current = it->first;
			
			for (auto dir = 0; dir != BUILDINGDIRECTIONS_TOTAL; dir++) { //iterating through 
				auto neighbor = current->get_neighbor(BUILDINGDIRECTION(dir));
				if (neighbor == nullptr) continue;

				if (visited.find(neighbor) == visited.end())
				{
					//if this new building hasn't been visited
					depth_search[depth + 1].emplace_back(std::pair<Building*, Building*>(neighbor, it->first));
					visited.insert(neighbor);
					if (neighbor == source)
					{
						success = true;
						distance = depth + 1;
					}
				}
			}
		}
	}
	// target not found
	if (distance == 0) return false;

	//collect path from source to target
	auto current = source;
	for (auto depth = distance; depth > 0; depth--)
	{
		for (auto& it : depth_search[depth])
		{
			if (it.first == current)
			{
				//add to checkpoints
				auto p = it.second->get_coords();
				p.x += TILE_WIDTH / 2;
				p.y += TILE_HEIGHT / 2;
				mCheckpoints.emplace_back(p);
				current = it.second;
			}
		}
	}
	return true;
}