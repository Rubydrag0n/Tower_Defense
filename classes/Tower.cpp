#include <SDL.h>

#include "Menu.h"
#include "Tower.h"
#include "ConfigFile.h"
#include "Enemy.h"
#include "EntityHandler.h"
#include "SDL_setup.h"
#include "LayerHandler.h"
#include "TowerWindow.h"
#include "Carriage.h"


Tower::Tower(const std::string& tower_name, const SDL_Point coords, Level *level) : Building(tower_name, coords, level), mTower_name(tower_name)
{
	const auto tower_stats_section = mTower_name + "/stats";
	mDamage.set_damages(gConfig_file->value_or_zero(tower_stats_section, "phys"),
		gConfig_file->value_or_zero(tower_stats_section, "magic"),
		gConfig_file->value_or_zero(tower_stats_section, "fire"),
		gConfig_file->value_or_zero(tower_stats_section, "water"),
		gConfig_file->value_or_zero(tower_stats_section, "elec"));

	mRange = gConfig_file->value(tower_stats_section, "range");
	mAttack_speed = gConfig_file->value(tower_stats_section, "attackspeed");
	mProjectile_speed = gConfig_file->value(tower_stats_section, "projectilespeed");
	mProjectile_name.assign(gConfig_file->value(tower_stats_section, "projectile_name"));
	mAttack_cooldown = int(60 / mAttack_speed);
	const auto sprite_path = std::string(gConfig_file->value("rangeindicator", "path"));
	mRange_indicator_sprite = gTextures->get_texture(sprite_path);

	SDL_Rect rect;
	rect.x = mCoords.x;
	rect.y = mCoords.y - 200;
	rect.w = 200;
	rect.h = 200;
	mWindow = new TowerWindow(rect, this);

	new Carriage("carriage", mLevel, reinterpret_cast<Building*>(mLevel->get_main_building()), this);
}

void Tower::render()
{
	Building::render();

	const auto temp_window = dynamic_cast<TowerWindow*>(mWindow);

	if(is_clicked())
	{
		SDL_Rect dest;
		//const auto offset = mRange / 800;

		dest.x = int(get_coords().x - mRange/2 + TILE_WIDTH/2);
		dest.y = int(get_coords().y - mRange/2 + TILE_HEIGHT/2);
		dest.w = int(mRange);
		dest.h = int(mRange);
		gLayer_handler->render_to_layer(mRange_indicator_sprite, LAYERS::WINDOWS, nullptr, &dest);

		temp_window->get_upgrade_damage_button()->enable();
		temp_window->get_upgrade_damage_button()->set_rendering_enabled(true);
		temp_window->get_upgrade_range_button()->enable();
		temp_window->get_upgrade_range_button()->set_rendering_enabled(true);
		temp_window->get_upgrade_attackspeed_button()->enable();
		temp_window->get_upgrade_attackspeed_button()->set_rendering_enabled(true);
	}
	else
	{
		temp_window->get_upgrade_damage_button()->disable();
		temp_window->get_upgrade_damage_button()->set_rendering_enabled(false);
		temp_window->get_upgrade_range_button()->disable();
		temp_window->get_upgrade_range_button()->set_rendering_enabled(false);
		temp_window->get_upgrade_attackspeed_button()->disable();
		temp_window->get_upgrade_attackspeed_button()->set_rendering_enabled(false);
	}
}

void Tower::on_tick()
{
	Building::on_tick();
	// try to shoot
	const auto all_enemies = gEntity_handler->get_entities_of_type(ENTITYTYPE::ENEMY);
	if (mElapsed_ticks % mAttack_cooldown == 0 && !this->mIdle)
	{
		const auto end = all_enemies->end();
		for (auto it = all_enemies->begin(); it != end; ++it)
		{
			const auto enemy = dynamic_cast<Enemy*>(*it);
			if (enemy_in_range(enemy, mRange, mCoords) && !enemy->is_dead())
			{
				this->create_shot(enemy);
				break;
			}
		}
	}
}

void Tower::upgrade(const std::string& tower_upgrade_section)
{
	Building::upgrade(tower_upgrade_section);
	mDamage.add(gConfig_file->value_or_zero(tower_upgrade_section, "phys"),
		gConfig_file->value_or_zero(tower_upgrade_section, "magic"),
		gConfig_file->value_or_zero(tower_upgrade_section, "fire"),
		gConfig_file->value_or_zero(tower_upgrade_section, "water"),
		gConfig_file->value_or_zero(tower_upgrade_section, "elec"));

	mRange += gConfig_file->value_or_zero(tower_upgrade_section, "range");
	mAttack_speed += gConfig_file->value_or_zero(tower_upgrade_section, "attackspeed");
	mProjectile_speed += gConfig_file->value_or_zero(tower_upgrade_section, "projectilespeed");
	mProjectile_name.assign(gConfig_file->value(tower_upgrade_section, "projectile_name"));
	mAttack_cooldown = 60 / mAttack_speed;
}

void Tower::upgrade_damage()
{
	const auto tower_upgrade_section = mTower_name + "/upgradeDamage";
	Building::upgrade(tower_upgrade_section);
	mDamage.add(gConfig_file->value_or_zero(tower_upgrade_section, "phys"),
		gConfig_file->value_or_zero(tower_upgrade_section, "magic"),
		gConfig_file->value_or_zero(tower_upgrade_section, "fire"),
		gConfig_file->value_or_zero(tower_upgrade_section, "water"),
		gConfig_file->value_or_zero(tower_upgrade_section, "elec"));
	mCount_of_little_upgrades++;
}

void Tower::upgrade_range()
{
	const auto tower_upgrade_section = mTower_name + "/upgradeRange";
	Building::upgrade(tower_upgrade_section);
	mRange += gConfig_file->value(tower_upgrade_section, "range");
	mCount_of_little_upgrades++;
}

void Tower::upgrade_attack_speed()
{
	const auto tower_upgrade_section = mTower_name + "/upgradeAttackspeed";
	Building::upgrade(tower_upgrade_section);
	mAttack_speed += gConfig_file->value(tower_upgrade_section, "attackspeed");
	mAttack_cooldown = 60 / mAttack_speed;
	mCount_of_little_upgrades++;
}

bool Tower::enemy_in_range(Enemy* enemy, const double radius, const SDL_Point center)
{
	const auto x_div = center.x - enemy->get_position().x;
	const auto y_div = center.y - enemy->get_position().y;
	const auto dist_to_enemy = sqrt(x_div * x_div + y_div * y_div);
	return dist_to_enemy <= radius;
}

std::string Tower::get_projectile_name() const
{
	return mProjectile_name;
}

double Tower::get_projectile_speed() const
{
	return this->mProjectile_speed;
}

Damage Tower::get_damage() const
{
	return this->mDamage;
}

BUILDINGTYPE Tower::get_building_type()
{
	return BUILDINGTYPE::TOWER;
}