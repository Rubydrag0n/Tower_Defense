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
	auto building_sprite_section = mName + "/sprite";
	auto building_stats_section = mName + "/stats";
	mSprite_path = std::string(gConfig_file->Value(building_sprite_section, "path"));
	//load texture and the size of the image from the config file
	mSprite = gTextures->get_texture(mSprite_path);
	mSprite_dimensions.w = gConfig_file->Value(building_sprite_section, "image_width");
	mSprite_dimensions.h = gConfig_file->Value(building_sprite_section, "image_height");
	mSprite_dimensions.x = 0;
	mSprite_dimensions.y = 0;

	this->mMaintenance = new Resources();
	this->mConstruction_costs = new Resources();
	this->mCurrent_resources = new Resources();
	auto resource_limit = new Resources();

	//set the maintenance costs of the building
	mMaintenance->set_resources(gConfig_file->Value(building_stats_section, "goldMain"),
		gConfig_file->Value(building_stats_section, "woodMain"),
		gConfig_file->Value(building_stats_section, "stoneMain"),
		gConfig_file->Value(building_stats_section, "ironMain"),
		gConfig_file->Value(building_stats_section, "energyMain"),
		gConfig_file->Value(building_stats_section, "waterMain"),
		gConfig_file->Value(building_stats_section, "foodMain"));

	mConstruction_costs->set_resources(gConfig_file->Value(building_stats_section, "goldcosts"),
		gConfig_file->Value(building_stats_section, "woodcosts"),
		gConfig_file->Value(building_stats_section, "stonecosts"),
		gConfig_file->Value(building_stats_section, "ironcosts"),
		gConfig_file->Value(building_stats_section, "energycosts"),
		gConfig_file->Value(building_stats_section, "watercosts"),
		gConfig_file->Value(building_stats_section, "foodcosts"));

	resource_limit->set_resources(gConfig_file->Value(building_stats_section, "goldLimit"),
		gConfig_file->Value(building_stats_section, "woodLimit"),
		gConfig_file->Value(building_stats_section, "stoneLimit"),
		gConfig_file->Value(building_stats_section, "ironLimit"),
		gConfig_file->Value(building_stats_section, "energyLimit"),
		gConfig_file->Value(building_stats_section, "waterLimit"),
		gConfig_file->Value(building_stats_section, "foodLimit"));

	//building starts without resources
	mCurrent_resources->set_empty();
	mCurrent_resources->set_limit(resource_limit);

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
	int i = gConfig_file->Value(building_stats_section, "tile");
	mTile_to_build_on = static_cast<TILETYPES>(i);

	for (auto i = 0; BUILDINGDIRECTION(i) < BUILDINGDIRECTION::BUILDINGDIRECTIONS_TOTAL; i++)
	{
		this->mSurrounding_buildings[BUILDINGDIRECTION(i)] = nullptr;
	}
	mLevel->set_building_matrix(mCoords.x / TILE_WIDTH, mCoords.y / TILE_HEIGHT, this);
}

Building::~Building()
{
	delete mWindow;
	//don't destroy texture, handled by texture class
}

void Building::demolish()
{
	mLevel->get_resources()->add(&(*mConstruction_costs/2));
	SDL_Point p;
	auto grid_offset_x = (mCoords.x) % TILE_WIDTH;
	auto grid_offset_y = (mCoords.y) % TILE_HEIGHT;
	p.x = mCoords.x - grid_offset_x;
	p.y = mCoords.y - grid_offset_y;
	auto tile_x = mCoords.x / 64;
	auto tile_y = mCoords.y / 64;
	mLevel->set_map_matrix(tile_x, tile_y, mTile_to_build_on);
	mLevel->set_building_matrix(mCoords.x / TILE_WIDTH, mCoords.y / TILE_HEIGHT, nullptr);
}

//TODO: magic numbers
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
		if(mCoords.y < 200)
		{
			window_rect.y = mCoords.y + mSprite_dimensions.h;
		}
		if(mCoords.x > 1080)
		{
			window_rect.x = mCoords.x + 64 - 200;
		}
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
	//TODO: magic number, framerate might not always be 60
	if(mElapsed_ticks % 60 == 0)
	{
		mIdle = !this->mCurrent_resources->sub(mMaintenance);
	}
	mElapsed_ticks++;
}


void Building::set_maintenance(Resources* new_maintenance)
{
	mMaintenance = new Resources(new_maintenance);
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

Resources* Building::get_maintenance() const
{
	return this->mMaintenance;
}

bool Building::get_idle()
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

void Building::add_resources(Resources * r)
{
	this->mCurrent_resources->add(r);
}

bool Building::transfer_resources_in(Resources * r)
{
	return this->mCurrent_resources->transfer(r);
}

bool Building::transfer_resources_out(Resources * r)
{
	return r->transfer(this->mCurrent_resources);
}

void Building::set_idle(bool value)
{
	this->mIdle = value;
}

Building* Building::get_neighbour(BUILDINGDIRECTION dir)
{
	return this->mSurrounding_buildings[dir];
}

void Building::set_neighbour(BUILDINGDIRECTION dir, Building* building)
{
	this->mSurrounding_buildings[dir] = building;
}
