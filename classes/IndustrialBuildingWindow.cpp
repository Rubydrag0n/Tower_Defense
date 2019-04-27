#include "IndustrialBuildingWindow.h"
#include "IndustrialBuilding.h"

IndustrialBuildingWindow::IndustrialBuildingWindow(SDL_Rect dim, IndustrialBuilding* industrial_building) : BuildingWindow(dim, industrial_building)
{
	SDL_Rect dest;
	dest.h = 0;
	dest.w = 0;
	dest.x = mDim.x + 20;
	dest.y = mDim.y + 130;

	mProduction_values = new Text*[RESOURCES_TOTAL];

	mProduction_headline = new Text("        Production", dest, WINDOWBUTTONS, mText_color, this);
	for (auto i = 0; i < RESOURCES_TOTAL; ++i)
	{
		dest.y += 20;
		mResource_names[i] = new Text(Resources::get_name(RESOURCETYPES(i)), dest, WINDOWS, mText_color, this);
		mProduction_values[i] = new Text(std::to_string(mBuilding->get_produce()->get_display_resources().get_resource(RESOURCETYPES(i))), dest, WINDOWBUTTONS, mText_color, this);
		mProduction_values[i]->add_x_dim(60);
	}
}

IndustrialBuildingWindow::~IndustrialBuildingWindow()
{
	for (auto i = 0; i < RESOURCES_TOTAL; i++)
	{
		delete mProduction_values[i];
	}
	delete[] mProduction_values;
}

void IndustrialBuildingWindow::render()
{
	BuildingWindow::render();
	for (auto i = 0; i < RESOURCES_TOTAL; ++i)
	{
		mProduction_values[i]->set_text(std::to_string(mBuilding->get_produce()->get_display_resources().get_resource(RESOURCETYPES(i))));
	}
}



