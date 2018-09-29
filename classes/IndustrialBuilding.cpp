#include "IndustrialBuilding.h"
#include "ConfigFile.h"


IndustrialBuilding::IndustrialBuilding(std::string industrial_building_name, SDL_Point coords, Level *level) : Building(industrial_building_name, coords, level)
{
	//set the ressources that are produced per second
	mRessources_produced_per_second.set_resources(gConfig_file->Value(industrial_building_name + "/stats", "goldproduction"),
		gConfig_file->Value(industrial_building_name + "/stats", "woodproduction"),
		gConfig_file->Value(industrial_building_name + "/stats", "stoneproduction"),
		gConfig_file->Value(industrial_building_name + "/stats", "ironproduction"),
		gConfig_file->Value(industrial_building_name + "/stats", "energyproduction"),
		gConfig_file->Value(industrial_building_name + "/stats", "waterproduction"),
		gConfig_file->Value(industrial_building_name + "/stats", "foodproduction"));
}

IndustrialBuilding::~IndustrialBuilding()
{
}


void IndustrialBuilding::render()
{
	Building::render();
}

void IndustrialBuilding::on_tick()
{
	Building::on_tick();
	if (mElapsed_ticks % 60 == 0)
	{
		if(!this->mIdle)
		{
			mLevel->get_resources()->add(&mRessources_produced_per_second);
		}
	}
}
