#include "IndustrialBuilding.h"
#include "ConfigFile.h"


IndustrialBuilding::IndustrialBuilding(std::string industrialBuilding_name, SDL_Point coords, Level *level) : Building(industrialBuilding_name, coords, level)
{
	ConfigFile cf("config/game.cfg");
	
	//set the ressources that are produced per second
	mRessources_produced_per_second.set_ressources(cf.Value(industrialBuilding_name + "/stats", "goldproduction"),
		cf.Value(industrialBuilding_name + "/stats", "woodproduction"),
		cf.Value(industrialBuilding_name + "/stats", "stoneproduction"),
		cf.Value(industrialBuilding_name + "/stats", "ironproduction"),
		cf.Value(industrialBuilding_name + "/stats", "energyproduction"),
		cf.Value(industrialBuilding_name + "/stats", "waterproduction"),
		cf.Value(industrialBuilding_name + "/stats", "foodproduction"));

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
