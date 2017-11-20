#include "IndustrialBuilding.h"
#include "ConfigFile.h"


IndustrialBuilding::IndustrialBuilding(std::string industrial_building_name, SDL_Point coords, Level *level) : Building(industrial_building_name, coords, level)
{
	ConfigFile cf("config/game.cfg");
	
	//set the ressources that are produced per second
	mRessources_produced_per_second.set_ressources(cf.Value(industrial_building_name + "/stats", "goldproduction"),
		cf.Value(industrial_building_name + "/stats", "woodproduction"),
		cf.Value(industrial_building_name + "/stats", "stoneproduction"),
		cf.Value(industrial_building_name + "/stats", "ironproduction"),
		cf.Value(industrial_building_name + "/stats", "energyproduction"),
		cf.Value(industrial_building_name + "/stats", "waterproduction"),
		cf.Value(industrial_building_name + "/stats", "foodproduction"));
}

IndustrialBuilding::~IndustrialBuilding()
{
}

void IndustrialBuilding::render() const
{
	Building::render();
}

void IndustrialBuilding::update()
{
	Building::update();
	mLevel->get_ressources()->add(&mRessources_produced_per_second);
}
