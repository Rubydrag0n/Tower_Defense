#pragma once
#include "BuildingWindow.h"

class IndustrialBuilding;

class IndustrialBuildingWindow : public BuildingWindow
{
public:
	IndustrialBuildingWindow(SDL_Rect dim, IndustrialBuilding* industrial_building);
	~IndustrialBuildingWindow();

	void render() override;

private:
	Text** mProduction_values;
	Text* mProduction_headline;
};
