#pragma once
#include <SDL.h>
#include <string>

#include "Clickable.h"
#include "LTexture.h"
#include "Entity.h"
#include "BuildingWindow.h"
#include "Menu.h"
#include "Enums.h"

class Carriage;
class Level;
class Resources;
class Window;
class Production;

class Building : public Clickable, public Entity
{
public:
	Building(std::string building_name, SDL_Point coords, Level *level, LAYERS click_layer, LAYERS render_layer);
	~Building();

	void render() override; //render the picture of the building
	void on_tick() override; //takes resources for maintenance
	void demolish() const;
	virtual void upgrade(const std::string& building_upgrade_section); //upgrades building

	SDL_Point get_coords() const;
	SDL_Rect get_dimensions() const;

	Resources* get_maintenance() const;
	void set_maintenance(Resources* maintenance);

	Resources* get_produce() const;
	void set_produce(Resources* produce);

	void set_coords(SDL_Point coords);
	void set_idle(bool value);
	bool get_idle() const;
	void set_building_level(std::string building_level);
	std::string get_building_level() const;
	int get_count_of_little_upgrades() const;
	std::string get_name() const;

	ENTITYTYPE get_type() override;
	virtual BUILDINGTYPE get_building_type() = 0;

	//returns the buildings neighboring this one
	std::vector<Building*> get_neighbors() const;
	void update_neighbors();

	Resources* get_current_resources() const;
	void add_resources(Resources* r) const;

	//puts resources into this building from r
	//returns true if r is empty afterwards
	bool transfer_resources_in(Resources* r) const;
	//puts resources into r from this building
	//returns true if this building is empty afterwards
	bool transfer_resources_out(Resources* r) const;

	void transfer_resources(Resources* r, Production* production, bool reverse) const;

	void on_click(int mouse_x, int mouse_y) override;

protected:
	SDL_Point mCoords;
	SDL_Point mBuilding_dimensions;
	Resources* mMaintenance;
	Resources* mProduce;
	LTexture *mSprite; //texture
	SDL_Rect mSprite_dimensions; //size of the texture
	Level *mLevel;

	bool mIdle; // if the player doesn't have enough res to maintain the building the building doesn't do anything
	int mElapsed_ticks;
	std::string mName;
	std::string mSprite_path;
	Resources* mConstruction_costs;
	TILETYPES mTile_to_build_on;
	BuildingWindow *mWindow;	//the window in which the stats and stuff of the tower can be displayed

	//a vector holding the neighbors of this building
	std::vector<Building*> mSurrounding_buildings;

	//how many resources the building has
	Resources* mCurrent_resources;
	Production* mProducing;

	std::string mBuilding_level; //level of the building, can be changed by GUpgrades
	int mCount_of_little_upgrades; //count of the little upgrades as a requirement for big upgrades

	Carriage* mCarriage;
};
