#include "Building.h"
#include "ConfigFile.h"
#include "SDL_setup.h"
#include "Level.h"
#include "Window.h"
#include "LayerHandler.h"

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


void Building::render()
{
	SDL_Rect dest;

	dest.x = mCoords.x;
	dest.y = mCoords.y;
	dest.w = mSprite_dimensions.w;
	dest.h = mSprite_dimensions.h;
	
	gLayer_handler->render_to_layer(mSprite, LAYERS::BUILDINGS, nullptr, &dest);

	if(this->get_clicked())
	{
		SDL_Rect rect;
		rect.x = mCoords.x;
		rect.y = mCoords.y - 200;
		rect.w = 200;
		rect.h = 200;
		mWindow->set_dim(rect);
		mWindow->render();
	}
	/*else
	{
		mWindow->delete_button();
	}*/
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
	printf("Building clicked: %s\n", this->mName.c_str());
	this->set_clicked(true);
}

void Building::on_mouse_over(int mouse_x, int mouse_y)
{
	printf("Building mouse over: %s\n", this->mName.c_str());
}

void Building::on_right_click(int mouse_x, int mouse_y)
{
	printf("Building right click: %s\n", this->mName.c_str());
}

void Building::on_middle_click(int mouse_x, int mouse_y)
{
	printf("Building middle click: %s\n", this->mName.c_str());
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


