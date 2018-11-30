#pragma once
#include <SDL.h>
#include <string>

#include "Resources.h"
#include "Clickable.h"
#include "LTexture.h"
#include "Entity.h"
#include "Window.h"
#include "BuildingWindow.h"
#include "Level.h"
#include "Menu.h"
#include "Enums.h"

class Level;

class Building : public Clickable, public Entity
{
public:
	Building(std::string building_name, SDL_Point coords, Level *level);
	~Building();

	void render() override; //render the picture of the building
	void on_tick() override; //takes ressources for maintenance
	void demolish();
	virtual void upgrade(std::string building_upgrade_section); //upgrades building

	SDL_Point get_coords() const;
	SDL_Rect get_dimensions() const;
	Resources* get_maintenance() const;
	void set_maintenance(Resources* maintenance);
	void set_coords(SDL_Point coords);
	void set_idle(bool value);
	bool get_idle();
	int get_building_level();
	int get_building_max_level();
	std::string get_name();

	ENTITYTYPE get_type() override;
	virtual BUILDINGTYPE get_building_type() = 0;

	//returns the building in the neighbouring tile or nullptr if it doesn't exist
	Building* get_neighbour(BUILDINGDIRECTION dir);
	//sets the building in the given direction to building
	void set_neighbour(BUILDINGDIRECTION dir, Building* building);

	Resources* get_current_resources() const;
	void add_resources(Resources* r);

	//puts resources into this building from r
	//returns true if r is empty afterwards
	bool transfer_resources_in(Resources* r);
	//puts resources into r from this building
	//returns true if this building is empty afterwards
	bool transfer_resources_out(Resources* r);

	void on_click(int mouse_x, int mouse_y) override;

protected:
	SDL_Point mCoords;
	Resources* mMaintenance;
	LTexture *mSprite; //texture
	SDL_Rect mSprite_dimensions; //size of the texture
	Level *mLevel;

	bool mIdle; // if the player doesnt have enough res to maintain the building the building doesnt do anything
	int mElapsed_ticks;
	std::string mName;
	std::string mSprite_path;
	Resources* mConstruction_costs;
	TILETYPES mTile_to_build_on;
	BuildingWindow *mWindow;	//the window in which the stats and stuff of the tower can be displayed

	//this map holds pointers to the neighbouring buildings or nullptrs
	//it is kept up to date by the level
	std::map<BUILDINGDIRECTION, Building*> mSurrounding_buildings;

	//how many resources the building has
	Resources* mCurrent_resources;

	int mBuilding_level; //level of the building, can be raised by upgrades
	int mBuilding_max_level; //max level of this building
	int mCount_of_little_upgrade; //count of the little upgrades as a requiremnt for big upgrades
};
