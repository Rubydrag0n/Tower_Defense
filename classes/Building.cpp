#include "Building.h"
#include "ConfigFile.h"
#include "SDL_setup.h"
#include <SDL.h>
#include "Level.h"
#include "Window.h"

Building::Building(std::string building_name, SDL_Point coords, Level* level)
{
	ConfigFile cf("config/game.cfg");

	mName = building_name;
	mSprite_path = std::string(cf.Value(mName + "/sprite", "path"));

	//load texture and the size of the image from the config file
	mSprite = gTextures->get_texture(mSprite_path);
	mSprite_dimensions.w = cf.Value(building_name + "/sprite", "image_width");
	mSprite_dimensions.h = cf.Value(building_name + "/sprite", "image_height");
	mSprite_dimensions.x = 0;
	mSprite_dimensions.y = 0;

	//set the construction costs of the building
	mConstruction_costs.set_ressources(cf.Value(building_name + "/stats", "goldcosts"),
									   cf.Value(building_name + "/stats", "woodcosts"),
									   cf.Value(building_name + "/stats", "stonecosts"),
									   cf.Value(building_name + "/stats", "ironcosts"),
									   cf.Value(building_name + "/stats", "energycosts"),
									   cf.Value(building_name + "/stats", "watercosts"),
									   cf.Value(building_name + "/stats", "foodcosts"));

	//set the maintenance costs of the building
	mMaintenance.set_ressources(cf.Value(building_name + "/stats", "goldMain"),
		cf.Value(building_name + "/stats", "woodMain"),
		cf.Value(building_name + "/stats", "stoneMain"),
		cf.Value(building_name + "/stats", "ironMain"),
		cf.Value(building_name + "/stats", "energyMain"),
		cf.Value(building_name + "/stats", "waterMain"),
		cf.Value(building_name + "/stats", "foodMain"));

	mLevel = level;
	mCoords = coords;

	SDL_Rect clickable;
	clickable.x = coords.x - mSprite_dimensions.w/2;
	clickable.y = coords.y - mSprite_dimensions.h/2;
	clickable.w = mSprite_dimensions.w;
	clickable.h = mSprite_dimensions.h;
	this->set_clickable_space(clickable);
}

Building::~Building()
{
	//don't destroy texture, handled by texture class
}

//render the picture of the building
void Building::render() const
{
	SDL_Rect dest;
	dest.x = mCoords.x - mSprite_dimensions.w / 2;
	dest.y = mCoords.y - mSprite_dimensions.h / 2;
	dest.w = mSprite_dimensions.w;
	dest.h = mSprite_dimensions.h;

	SDL_RenderCopy(gRenderer, mSprite, &mSprite_dimensions, &dest);
}

void Building::update()
{
	mLevel->get_ressources()->sub(&mMaintenance);
}

void Building::place()
{
	mLevel->get_ressources()->sub(&mConstruction_costs);
}

void Building::set_maintenance(Ressources new_maintenance)
{
	mMaintenance = new_maintenance;
}

void Building::on_click(int mouse_x, int mouse_y)
{
	printf("Building clicked: %s\n", this->mName.c_str());
}

void Building::on_mouse_over(int mouse_x, int mouse_y)
{
	printf("Building mouse over: %s\n", this->mName.c_str());
	SDL_Rect rect;
	rect.x = mouse_x;
	rect.y = mouse_y;
	rect.w = 200;
	rect.h = 200;
	auto w = new Window(rect);
	w->render();
	delete w;
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

Ressources Building::get_construction_costs() const
{
	return mConstruction_costs;
}

Ressources Building::get_maintenance() const
{
	return mMaintenance;
}


