#include <SDL.h>

#include "Menu.h"
#include "Tower.h"
#include "ConfigFile.h"
#include "Enemy.h"
#include "EntityHandler.h"
#include "SDL_setup.h"
#include "LayerHandler.h"


Tower::Tower(std::string tower_name, SDL_Point coords, Level *level) : Building(tower_name, coords, level)
{
	mTower_name = tower_name;
	auto tower_stats_section = mTower_name + "/stats";
	mDamage.set_damages(gConfig_file->Value(tower_stats_section, "phys"),
		gConfig_file->Value(tower_stats_section, "magic"),
		gConfig_file->Value(tower_stats_section, "fire"),
		gConfig_file->Value(tower_stats_section, "water"),
		gConfig_file->Value(tower_stats_section, "elec"));

	mRange = gConfig_file->Value(tower_stats_section, "range");
	mAttack_speed = gConfig_file->Value(tower_stats_section, "attackspeed");
	mProjectile_speed = gConfig_file->Value(tower_stats_section, "projectilespeed");
	mProjectile_name.assign(gConfig_file->Value(tower_stats_section, "projectile_name"));
	mAttack_cooldown = 60 / mAttack_speed;
	auto sprite_path = std::string(gConfig_file->Value("rangeindicator", "path"));
	mRange_indicator_sprite = gTextures->get_texture(sprite_path);
}

Tower::~Tower()
{
}

void Tower::render()
{
	Building::render();
	if(get_clicked())
	{
		SDL_Rect dest;
		auto dest_offset = mRange / 800;
		dest.x = get_coords().x - 400 * dest_offset;
		dest.y = get_coords().y - 400 * dest_offset;
		dest.h = 800 * dest_offset;
		dest.w = 800 * dest_offset;
		gLayer_handler->render_to_layer(mRange_indicator_sprite, LAYERS::WINDOWS, nullptr, &dest);
	}
}

void Tower::on_tick()
{
	Building::on_tick();
	// try to shoot
	auto all_enemies = gEntity_handler->get_entities_of_type(ENTITYTYPE::ENEMY);
	if (mElapsed_ticks % mAttack_cooldown == 0 && !this->mIdle)
	{
		auto end = all_enemies->end();
		for (auto it = all_enemies->begin(); it != end; ++it)
		{
			auto enemy = dynamic_cast<Enemy*>(*it);
			if (enemy_in_range(enemy, mRange, mCoords) && !enemy->is_dead())
			{
				this->create_shot(enemy);
				break;
			}
		}
	}
}

BUILDINGTYPE Tower::get_building_type()
{
	return BUILDINGTYPE::TOWER;
}

bool Tower::enemy_in_range(Enemy* enemy, double radius, SDL_Point center)
{
	auto x_div = center.x - enemy->get_position().x;
	auto y_div = center.y - enemy->get_position().y;
	auto dist_to_enemy = sqrt(x_div * x_div + y_div * y_div);
	if(dist_to_enemy <= radius)
	{
		return true;
	}
	return false;
}

std::string Tower::get_projectile_name() const
{
	return mProjectile_name;
}

double Tower::get_projectile_speed() const
{
	return this->mProjectile_speed;
}

Damage Tower::get_damage()
{
	return this->mDamage;
}