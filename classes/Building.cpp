#include "Building.h"
#include <utility>
#include "ConfigFile.h"
#include "SDL_setup.h"
#include "Level.h"
#include "Window.h"
#include "LayerHandler.h"
#include "MouseHandler.h"
#include "Production.h"
#include "Carriage.h"

Building::Building(std::string building_name, SDL_Point coords, Level* level) : mCoords{coords}, mSprite_dimensions{},
                                                                                mLevel{level},
                                                                                mName{std::move(building_name)},
                                                                                mWindow{nullptr},
																				mCarriage{nullptr}
{
	auto building_sprite_section = mName + "/sprite";
	auto building_stats_section = mName + "/stats";
	mSprite_path = std::string(gConfig_file->value(building_sprite_section, "path"));

	//load texture and the size of the image from the config file
	mSprite = gTextures->get_texture(mSprite_path);
	mSprite_dimensions.w = gConfig_file->value(building_sprite_section, "image_width");
	mSprite_dimensions.h = gConfig_file->value(building_sprite_section, "image_height");
	mSprite_dimensions.x = 0;
	mSprite_dimensions.y = 0;

	this->mMaintenance = new Resources();
	this->mProduce = new Resources();
	this->mConstruction_costs = new Resources();
	this->mCurrent_resources = new Resources();
	auto resource_limit = new Resources();

	//set the maintenance costs of the building
	mMaintenance->set_resources(gConfig_file->value_or_zero(building_stats_section, "goldMain"),
		gConfig_file->value_or_zero(building_stats_section, "woodMain"),
		gConfig_file->value_or_zero(building_stats_section, "stoneMain"),
		gConfig_file->value_or_zero(building_stats_section, "ironMain"),
		gConfig_file->value_or_zero(building_stats_section, "energyMain"),
		gConfig_file->value_or_zero(building_stats_section, "waterMain"),
		gConfig_file->value_or_zero(building_stats_section, "foodMain"));

	mConstruction_costs->set_resources(gConfig_file->value_or_zero(building_stats_section, "goldcosts"),
		gConfig_file->value_or_zero(building_stats_section, "woodcosts"),
		gConfig_file->value_or_zero(building_stats_section, "stonecosts"),
		gConfig_file->value_or_zero(building_stats_section, "ironcosts"),
		gConfig_file->value_or_zero(building_stats_section, "energycosts"),
		gConfig_file->value_or_zero(building_stats_section, "watercosts"),
		gConfig_file->value_or_zero(building_stats_section, "foodcosts"));

	resource_limit->set_resources(gConfig_file->value_or_zero(building_stats_section, "goldLimit"),
		gConfig_file->value_or_zero(building_stats_section, "woodLimit"),
		gConfig_file->value_or_zero(building_stats_section, "stoneLimit"),
		gConfig_file->value_or_zero(building_stats_section, "ironLimit"),
		gConfig_file->value_or_zero(building_stats_section, "energyLimit"),
		gConfig_file->value_or_zero(building_stats_section, "waterLimit"),
		gConfig_file->value_or_zero(building_stats_section, "foodLimit"));

	//set the resources that are produced per second
	mProduce->set_resources(gConfig_file->value_or_zero(building_stats_section, "goldproduction"),
		gConfig_file->value_or_zero(building_stats_section, "woodproduction"),
		gConfig_file->value_or_zero(building_stats_section, "stoneproduction"),
		gConfig_file->value_or_zero(building_stats_section, "ironproduction"),
		gConfig_file->value_or_zero(building_stats_section, "energyproduction"),
		gConfig_file->value_or_zero(building_stats_section, "waterproduction"),
		gConfig_file->value_or_zero(building_stats_section, "foodproduction"));

	//building starts without resources
	mCurrent_resources->set_empty();
	mCurrent_resources->set_limit(resource_limit);

	mBuilding_level = "";
	mCount_of_little_upgrades = 0;

	mElapsed_ticks = 0;

	SDL_Rect clickable;
	clickable.x = coords.x;
	clickable.y = coords.y;
	clickable.w = mSprite_dimensions.w;
	clickable.h = mSprite_dimensions.h;
	this->set_clickable_space(clickable);
	mIdle = false;

	//set the mouse over window up with initial values

	int i = gConfig_file->value(building_stats_section, "tile");
	mTile_to_build_on = static_cast<TILETYPES>(i);

	for (auto j = 0; BUILDINGDIRECTION(j) < BUILDINGDIRECTION::BUILDINGDIRECTIONS_TOTAL; j++)
	{
		this->mSurrounding_buildings[BUILDINGDIRECTION(j)] = nullptr;
	}
	mLevel->set_building_matrix(mCoords.x / TILE_WIDTH, mCoords.y / TILE_HEIGHT, this);

	//initialize what this building is producing and consuming
	mProducing = new Production(this);

	set_depth(CL_BUILDINGS);
}

Building::~Building()
{
	delete mWindow;
	delete mCarriage;
	//don't destroy texture, handled by texture class
}

void Building::demolish() const
{
	auto refund = *mConstruction_costs / 2;
	mLevel->get_resources()->add(&refund);

	const auto tile_x = mCoords.x / 64;
	const auto tile_y = mCoords.y / 64;
	mLevel->set_map_matrix(tile_x, tile_y, mTile_to_build_on);
	mLevel->set_building_matrix(mCoords.x / TILE_WIDTH, mCoords.y / TILE_HEIGHT, nullptr);
}

void Building::upgrade(const std::string& building_upgrade_section)
{
	const auto plus_maintenance = new Resources(gConfig_file->value_or_zero(building_upgrade_section, "goldMain"),
		gConfig_file->value_or_zero(building_upgrade_section, "woodMain"),
		gConfig_file->value_or_zero(building_upgrade_section, "stoneMain"),
		gConfig_file->value_or_zero(building_upgrade_section, "ironMain"),
		gConfig_file->value_or_zero(building_upgrade_section, "energyMain"),
		gConfig_file->value_or_zero(building_upgrade_section, "waterMain"),
		gConfig_file->value_or_zero(building_upgrade_section, "foodMain"));
	mMaintenance->add(plus_maintenance);

	const auto upgrade_cost_multiplier = mCount_of_little_upgrades * 2 + 1;
	const auto plus_construction = new Resources(gConfig_file->value_or_zero(building_upgrade_section, "goldcosts") * upgrade_cost_multiplier,
		gConfig_file->value_or_zero(building_upgrade_section, "woodcosts") * upgrade_cost_multiplier,
		gConfig_file->value_or_zero(building_upgrade_section, "stonecosts") * upgrade_cost_multiplier,
		gConfig_file->value_or_zero(building_upgrade_section, "ironcosts") * upgrade_cost_multiplier,
		gConfig_file->value_or_zero(building_upgrade_section, "energycosts") * upgrade_cost_multiplier,
		gConfig_file->value_or_zero(building_upgrade_section, "watercosts") * upgrade_cost_multiplier,
		gConfig_file->value_or_zero(building_upgrade_section, "foodcosts") * upgrade_cost_multiplier);
	mConstruction_costs->add(plus_construction);

	mLevel->get_resources()->sub(plus_construction);
}

void Building::render()
{
	SDL_Rect dest;

	dest.x = mCoords.x;
	dest.y = mCoords.y;
	dest.w = mSprite_dimensions.w;
	dest.h = mSprite_dimensions.h;
	
	gLayer_handler->render_to_layer(mSprite, LAYERS::BUILDINGS, nullptr, &dest);

	if(this->is_clicked())
	{		
		mWindow->set_rendering_enabled(true);
		mWindow->enable();
		mWindow->set_clicked(true);
	}
	else
	{
		//mWindow->set_rendering_enabled(false);
	}
}

void Building::on_tick()
{
	if(mElapsed_ticks % *gFrame_rate == 0)
	{
		mIdle = !this->mCurrent_resources->sub(mMaintenance); 

		if (!this->mIdle)
		{
			this->mCurrent_resources->add(mProduce);
		}
	}
	mElapsed_ticks++;
}

void Building::on_click(int mouse_x, int mouse_y)
{
	if(gMouse_handler->get_item_on_mouse() != nullptr)
	{
		if (gMouse_handler->get_item_on_mouse()->get_name_of_object() != mName)
		{
			this->set_clicked(true);
		}
	}
	else
	{
		this->set_clicked(true);
	}
}

void Building::set_maintenance(Resources* maintenance)
{
	delete mMaintenance;
	mMaintenance = new Resources(maintenance);
}

void Building::set_produce(Resources* produce)
{
	delete mProduce;
	mProduce = new Resources(produce);
}

void Building::set_coords(const SDL_Point coords)
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

Resources* Building::get_maintenance() const
{
	return this->mMaintenance;
}

Resources* Building::get_produce() const
{
	return this->mProduce;
}

bool Building::get_idle() const
{
	return this->mIdle;
}

ENTITYTYPE Building::get_type()
{
	return ENTITYTYPE::BUILDING;
}

Resources* Building::get_current_resources() const
{
	return this->mCurrent_resources;
}

void Building::add_resources(Resources * r) const
{
	this->mCurrent_resources->add(r);
}

bool Building::transfer_resources_in(Resources * r) const
{
	return this->mCurrent_resources->transfer(r);
}

bool Building::transfer_resources_out(Resources * r) const
{
	return r->transfer(this->mCurrent_resources);
}

void Building::transfer_resources(Resources* r, Production* production, const bool reverse) const
{
	for (auto i = 0; i < RESOURCES_TOTAL; i++)
	{
		switch(production->at(RESOURCETYPES(i)))
		{
		case NONE:
			//do nothing
			break;
		case PRODUCING:
			//if not reversed, transfer into r
			if (!reverse)
				r->transfer(RESOURCETYPES(i), mCurrent_resources->get_resource_pointer(RESOURCETYPES(i)));
			else
				mCurrent_resources->transfer(RESOURCETYPES(i), r->get_resource_pointer(RESOURCETYPES(i)));
			break;
		case CONSUMING:
			//if not reversed, transfer into building
			if (!reverse)
				mCurrent_resources->transfer(RESOURCETYPES(i), r->get_resource_pointer(RESOURCETYPES(i)));
			else
				r->transfer(RESOURCETYPES(i), mCurrent_resources->get_resource_pointer(RESOURCETYPES(i)));
			break;
		default: 
			;	//shouldn't get here
		}
	}
}

void Building::set_idle(const bool value)
{
	this->mIdle = value;
}

Building* Building::get_neighbor(const BUILDINGDIRECTION dir)
{
	return this->mSurrounding_buildings[dir];
}

void Building::set_neighbor(const BUILDINGDIRECTION dir, Building* building)
{
	this->mSurrounding_buildings[dir] = building;
}

std::string Building::get_building_level() const
{
	return mBuilding_level;
}

void Building::set_building_level(std::string building_level)
{
	mBuilding_level = building_level;
}

int Building::get_count_of_little_upgrades()
{
	return mCount_of_little_upgrades;
}


std::string Building::get_name() const
{
	return mName;
}


