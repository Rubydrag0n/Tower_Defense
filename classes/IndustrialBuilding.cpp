#include "IndustrialBuilding.h"
#include "ConfigFile.h"
#include "Menu.h"

IndustrialBuilding::IndustrialBuilding(std::string industrial_building_name, SDL_Point coords, Level *level) : Building(industrial_building_name, coords, level)
{
	SDL_Rect rect;
	rect.x = mCoords.x;
	rect.y = mCoords.y - 200;
	rect.w = 200;
	rect.h = 200;
	mWindow = new BuildingWindow(rect, this);
}

IndustrialBuilding::~IndustrialBuilding()
{
}

void IndustrialBuilding::render()
{
	Building::render();
}

BUILDINGTYPE IndustrialBuilding::get_building_type()
{
	return BUILDINGTYPE::INDUSTRIAL_BUILDING;
}

void IndustrialBuilding::on_tick()
{
	Building::on_tick();
	//TODO: magic number
	if (mElapsed_ticks % 60 == 0)
	{
		if(!this->mIdle)
		{
			this->mCurrent_resources->add(mProduce);
		}
	}
}
