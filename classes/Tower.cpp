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


Tower::Tower(const std::string& tower_name, const SDL_Point coords, Level *level, LAYERS click_layer, LAYERS render_layer) : Building(tower_name, coords, level, click_layer, render_layer), mTower_name(tower_name)
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


	mCarriage = new Carriage("carriage", mLevel, LAYERS::ENEMIES, reinterpret_cast<Building*>(mLevel->get_main_building()), this);
}

void Tower::render()
{
	Building::render();

	if(is_clicked())
	{
		SDL_Rect dest;
		//range is radius not diameter
		dest.x = int(get_coords().x - mRange + mSprite_dimensions.w/2.);
		dest.y = int(get_coords().y - mRange + mSprite_dimensions.h/2.);
		dest.w = int(mRange*2);
		dest.h = int(mRange*2);
		gLayer_handler->render_to_layer(mRange_indicator_sprite, LAYERS::OVERLAY, nullptr, &dest);
	}
}

void Tower::set_clicked(const bool value)
{
	//break;
	Clickable::set_clicked(value);
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

void Tower::on_click(int mouse_x, int mouse_y)
{
	SDL_Rect rect;
	rect.x = 1280;
	rect.y = 600;
	rect.w = 600;
	rect.h = 200;
	mLevel->get_menu()->set_building_window(new TowerWindow(rect, this));
	Building::on_click(mouse_x, mouse_y);
}


bool Tower::upgrade(const std::string& tower_upgrade_section)
{
	if(Building::upgrade(tower_upgrade_section))
	{
		mDamage.add(gConfig_file->value_or_zero(tower_upgrade_section, "phys"),
			gConfig_file->value_or_zero(tower_upgrade_section, "magic"),
			gConfig_file->value_or_zero(tower_upgrade_section, "fire"),
			gConfig_file->value_or_zero(tower_upgrade_section, "water"),
			gConfig_file->value_or_zero(tower_upgrade_section, "elec"));
		mRange += gConfig_file->value_or_zero(tower_upgrade_section, "range");
		mAttack_speed += gConfig_file->value_or_zero(tower_upgrade_section, "attackspeed");
		mProjectile_speed += gConfig_file->value_or_zero(tower_upgrade_section, "projectilespeed");
		mProjectile_name.assign(gConfig_file->value(tower_upgrade_section, "projectile_name"));
		mAttack_cooldown = int(*gFrame_rate / mAttack_speed);
		return true;
	}
	return false;
}

bool Tower::upgrade_damage()
{
	const auto tower_upgrade_section = "Tower/upgradeDamage";
	if(Building::upgrade(tower_upgrade_section))
	{
		mDamage.add(gConfig_file->value_or_zero(tower_upgrade_section, "phys"),
			gConfig_file->value_or_zero(tower_upgrade_section, "magic"),
			gConfig_file->value_or_zero(tower_upgrade_section, "fire"),
			gConfig_file->value_or_zero(tower_upgrade_section, "water"),
			gConfig_file->value_or_zero(tower_upgrade_section, "elec"));
		mCount_of_little_upgrades++;
		return true;
	}
	return false;
}

bool Tower::upgrade_range()
{
	const auto tower_upgrade_section = "Tower/upgradeRange";
	if(Building::upgrade(tower_upgrade_section))
	{
		mRange += gConfig_file->value(tower_upgrade_section, "range");
		mCount_of_little_upgrades++;
		return true;
	}
	return false;
}

bool Tower::upgrade_attack_speed()
{
	const auto tower_upgrade_section = "Tower/upgradeAttackspeed";
	if(Building::upgrade(tower_upgrade_section))
	{
		mAttack_speed += gConfig_file->value(tower_upgrade_section, "attackspeed");
		mAttack_cooldown = int(*gFrame_rate / mAttack_speed);
		mCount_of_little_upgrades++;
		return true;
	}
	return false;
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

double Tower::get_attack_speed() const
{
	return mAttack_speed;
}

double Tower::get_range() const
{
	return mRange;
}

int Tower::get_number_of_damage_upgrades() const
{
	return mNumber_of_damage_upgrades;
}

int Tower::get_number_of_attackspeed_upgrades() const
{
	return mNumber_of_attackspeed_upgrades;
}

int Tower::get_number_of_range_upgrades() const
{
	return mNumber_of_range_upgrades;
}

void Tower::increment_number_of_damage_upgrades()
{
	mNumber_of_damage_upgrades++;
}

void Tower::increment_number_of_attackspeed_upgrades()
{
	mNumber_of_attackspeed_upgrades++;
}

void Tower::increment_number_of_range_upgrades()
{
	mNumber_of_range_upgrades++;
}






