#include "Building.h"
#include "ConfigFile.h"
#include "SDL_setup.h"
#include "Level.h"
#include "Window.h"
#include "LayerHandler.h"
#include "MouseHandler.h"

Building::Building(std::string building_name, SDL_Point coords, Level* level)
{
	mName = building_name;
	mSprite_path = std::string(gConfig_file->Value(mName + "/sprite", "path"));
	//load texture and the size of the image from the config file
	mSprite = gTextures->get_texture(mSprite_path);
	mSprite_dimensions.w = gConfig_file->Value(building_name + "/sprite", "image_width");
	mSprite_dimensions.h = gConfig_file->Value(building_name + "/sprite", "image_height");
	mSprite_dimensions.x = 0;
	mSprite_dimensions.y = 0;

	//set the maintenance costs of the building
	mMaintenance.set_resources(gConfig_file->Value(building_name + "/stats", "goldMain"),
		gConfig_file->Value(building_name + "/stats", "woodMain"),
		gConfig_file->Value(building_name + "/stats", "stoneMain"),
		gConfig_file->Value(building_name + "/stats", "ironMain"),
		gConfig_file->Value(building_name + "/stats", "energyMain"),
		gConfig_file->Value(building_name + "/stats", "waterMain"),
		gConfig_file->Value(building_name + "/stats", "foodMain"));

	mConstruction_costs.set_resources(gConfig_file->Value(mName + "/stats", "goldcosts"),
		gConfig_file->Value(mName + "/stats", "woodcosts"),
		gConfig_file->Value(mName + "/stats", "stonecosts"),
		gConfig_file->Value(mName + "/stats", "ironcosts"),
		gConfig_file->Value(mName + "/stats", "energycosts"),
		gConfig_file->Value(mName + "/stats", "watercosts"),
		gConfig_file->Value(mName + "/stats", "foodcosts"));

	mElapsed_ticks = 0;

	mLevel = level;
	mCoords = coords;

	SDL_Rect clickable;
	clickable.x = coords.x;
	clickable.y = coords.y;
	clickable.w = mSprite_dimensions.w;
	clickable.h = mSprite_dimensions.h;
	this->set_clickable_space(clickable);
	mIdle = false;

	//set the mouse over window up with initial values
	SDL_Rect rect;
	rect.x = mCoords.x;
	rect.y = mCoords.y - 200;
	rect.w = 200;
	rect.h = 200;
	mWindow = new BuildingWindow(rect, this);
}

Building::~Building()
{
	delete(mWindow);
	//don't destroy texture, handled by texture class
}

void Building::demolish()
{
	mLevel->get_ressources()->add(&(mConstruction_costs/2));
}



void Building::render()
{
	SDL_Rect dest, window_rect, button_rect;

	dest.x = mCoords.x;
	dest.y = mCoords.y;
	dest.w = mSprite_dimensions.w;
	dest.h = mSprite_dimensions.h;
	
	gLayer_handler->render_to_layer(mSprite, LAYERS::BUILDINGS, nullptr, &dest);

	if(this->get_clicked())
	{
		window_rect.x = mCoords.x;
		window_rect.y = mCoords.y - 200;
		window_rect.w = 200;
		window_rect.h = 200;
		mWindow->set_dim(window_rect);
		mWindow->set_rendering_enabled(true);

		button_rect.x = window_rect.x;
		button_rect.y = window_rect.y;
		button_rect.w = 100;
		button_rect.h = 26;
		mWindow->get_demolish_button()->set_dimension(button_rect);
		mWindow->get_demolish_button()->set_rendering_enabled(true);
	}
	else
	{
		button_rect.x = 0;
		button_rect.y = 0;
		button_rect.h = 0;
		button_rect.w = 0;
		mWindow->set_rendering_enabled(false);
		mWindow->get_demolish_button()->set_rendering_enabled(false);
		mWindow->get_demolish_button()->set_dimension(button_rect);
	}
}

void Building::on_tick()
{
	if(mElapsed_ticks % 60 == 0)
	{
		mIdle = !mLevel->get_ressources()->sub(&mMaintenance);
	}
	mElapsed_ticks++;
}


void Building::set_maintenance(Resources new_maintenance)
{
	mMaintenance = new_maintenance;
}

void Building::on_click(int mouse_x, int mouse_y)
{
//	if(gMouse_handler->get_item_on_mouse() != nullptr)
//	{
	//	if (gMouse_handler->get_item_on_mouse()->get_name_of_object() != mName)
	//	{
			this->set_clicked(true);
	//	}
//	}

}

void Building::on_mouse_over(int mouse_x, int mouse_y)
{
}

void Building::on_right_click(int mouse_x, int mouse_y)
{	
}

void Building::on_middle_click(int mouse_x, int mouse_y)
{
}

void Building::set_coords(SDL_Point coords)
{
	mCoords = coords;
}

SDL_Point Building::get_coords() const
{
	return mCoords;
}

SDL_Rect Building::get_dimensions() const
{
	return mSprite_dimensions;
}

Resources Building::get_maintenance() const
{
	return mMaintenance;
}

bool Building::get_idle()
{
	return this->mIdle;
}

void Building::set_idle(bool value)
{
	this->mIdle = value;
}


