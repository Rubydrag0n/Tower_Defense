#include <SDL.h>
#include "Menu.h"
#include "Tower.h"
#include "ConfigFile.h"
#include "Enemy.h"
#include "EntityHandler.h"
#include "SDL_setup.h"
#include "LayerHandler.h"
#include "Carriage.h"


Tower::Tower(const std::string& tower_name, const SDL_Point coords, Level *level, LAYERS click_layer, LAYERS render_layer) : Building(tower_name, coords, level, click_layer, render_layer), mTower_name(tower_name)
{
	const auto tower_stats_section = mTower_name + "/stats";
	mExplosive_radius = gConfig_file->value_or_zero(mTower_name + "/stats", "explosiveradius");
	//set stat values for the tower
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

	//little upgrade buttons
	SDL_Rect dest;
	dest.x = mBuilding_window->get_dim().x + 30;
	dest.y = mBuilding_window->get_dim().y + 140;
	dest.w = 26;
	dest.h = 26;
	mUpgrade_damage_button = new UpgradeButton("testbutton", dest, this, mName, "Damage", WINDOWCONTENT, WINDOWCONTENT, mBuilding_window, BUILDINGWINDOWBUTTONIDS::UPGRADE_DAMAGE_BUTTON);
	dest.x += 56;
	mUpgrade_attackspeed_button = new UpgradeButton("testbutton", dest, this, mName, "Attackspeed", WINDOWCONTENT, WINDOWCONTENT, mBuilding_window, BUILDINGWINDOWBUTTONIDS::UPGRADE_ATTACKSPEED_BUTTON);
	dest.x += 56;
	mUpgrade_range_button = new UpgradeButton("testbutton", dest, this, mName, "Range", WINDOWCONTENT, WINDOWCONTENT, mBuilding_window, BUILDINGWINDOWBUTTONIDS::UPGRADE_RANGE_BUTTON);

	//number of little upgrades displayed
	SDL_Color text_color = { 0,0,0,0 };
	dest.h = 0;
	dest.w = 0;
	dest.x = mUpgrade_damage_button->get_dimension().x;
	dest.y = mUpgrade_damage_button->get_dimension().y + 30;
	mDamage_upgrade_number_texture = new Text(std::to_string(mNumber_of_damage_upgrades), dest, WINDOWCONTENT, text_color, mBuilding_window);
	mBuilding_window->add_text_to_window(mDamage_upgrade_number_texture);
	dest.x += 56;
	mAttackspeed_upgrade_number_texture = new Text(std::to_string(mNumber_of_attackspeed_upgrades), dest, WINDOWCONTENT, text_color, mBuilding_window);
	mBuilding_window->add_text_to_window(mAttackspeed_upgrade_number_texture);
	dest.x += 56;
	mRange_upgrade_number_texture = new Text(std::to_string(mNumber_of_attackspeed_upgrades), dest, WINDOWCONTENT, text_color, mBuilding_window);
	mBuilding_window->add_text_to_window(mRange_upgrade_number_texture);

	//turret stats-text displayed(const)
	dest.x = mBuilding_window->get_dim().x + 200;
	dest.y = mBuilding_window->get_dim().y + 20;
	dest.w = 0;	//setting these to 0 will not scale anything
	dest.h = 0;
	auto const dmg_text = new Text("Dmg: ", dest, WINDOWCONTENT, text_color, mBuilding_window);
	mBuilding_window->add_text_to_window(dmg_text);
	dest.y += 30;
	auto const as_text = new Text("AS: ", dest, WINDOWCONTENT, text_color, mBuilding_window);
	mBuilding_window->add_text_to_window(as_text);
	dest.y += 30;
	auto const range_text = new Text("Range: ", dest, WINDOWCONTENT, text_color, mBuilding_window);
	mBuilding_window->add_text_to_window(range_text);
	dest.y += 30;
	auto const explosive_radius_name = new Text("Explosive radius: ", dest, WINDOWCONTENT, text_color, mBuilding_window);
	mBuilding_window->add_text_to_window(explosive_radius_name);
	dest.y += 30;
	auto const damage_distribution_headline = new Text("Damage dist: ", dest, WINDOWS, text_color, mBuilding_window);
	mBuilding_window->add_text_to_window(damage_distribution_headline);
	dest.y += 30;
	mDamage_distribution_value = new Text("P: " + std::to_string(int(mDamage.get_phys_dmg()))
		+ " M: " + std::to_string(int(mDamage.get_magic_dmg()))
		+ " F: " + std::to_string(int(mDamage.get_fire_dmg()))
		+ " W: " + std::to_string(int(mDamage.get_water_dmg()))
		+ " E: " + std::to_string(int(mDamage.get_elec_dmg())), dest, WINDOWS, text_color, mBuilding_window);
	mBuilding_window->add_text_to_window(mDamage_distribution_value);

	//turret stats-numbers displayed(dynamic)
	dest.x = mBuilding_window->get_dim().x + 260;
	dest.y = mBuilding_window->get_dim().y + 20;
	mDmg_value = new Text(std::to_string(int(mDamage.get_dmg_sum())), dest, WINDOWS, text_color, mBuilding_window);
	mBuilding_window->add_text_to_window(mDmg_value);
	dest.y += 30;
	mAs_value = new Text(std::to_string(int(mAttack_speed)), dest, WINDOWS, text_color, mBuilding_window);
	mBuilding_window->add_text_to_window(mAs_value);
	dest.y += 30;
	mRange_value = new Text(std::to_string(int(mRange)), dest, WINDOWS, text_color, mBuilding_window);
	mBuilding_window->add_text_to_window(mRange_value);
	dest.y += 30;
	mExplosive_radius_value = new Text(std::to_string(int(mExplosive_radius)), dest, WINDOWCONTENT, text_color, mBuilding_window);
	mExplosive_radius_value->add_x_dim(70);
	mBuilding_window->add_text_to_window(mExplosive_radius_value);
}

Tower::~Tower() 
{
	delete mUpgrade_attackspeed_button;
	delete mUpgrade_range_button;
	delete mUpgrade_damage_button;
}

void Tower::render()
{
	Building::render();

	if (mLevel->get_menu()->get_building_window() != nullptr)
	{
		if (is_clicked() || mBuilding_window->is_clicked())
		{
			SDL_Rect dest;
			//range is radius not diameter
			dest.x = int(get_coords().x - mRange + mSprite_dimensions.w / 2.);
			dest.y = int(get_coords().y - mRange + mSprite_dimensions.h / 2.);
			dest.w = int(mRange * 2);
			dest.h = int(mRange * 2);
			gLayer_handler->render_to_layer(mRange_indicator_sprite, LAYERS::OVERLAY, nullptr, &dest);
		}
	}
}

void Tower::update_building_window()
{
	//updates texture: number of little upgrades
	mDamage_upgrade_number_texture->set_text(std::to_string(mNumber_of_damage_upgrades));
	mAttackspeed_upgrade_number_texture->set_text(std::to_string(mNumber_of_attackspeed_upgrades));
	mRange_upgrade_number_texture->set_text(std::to_string(mNumber_of_range_upgrades));
	auto dmg_value = std::to_string(int(mDamage.get_dmg_sum()));
	auto as_value = std::to_string(int(mAttack_speed));
	auto range_value = std::to_string(int(mRange));
	auto dmg_distribution_text = "P: " + std::to_string(int(mDamage.get_phys_dmg()))
		+ " M: " + std::to_string(int(mDamage.get_magic_dmg()))
		+ " F: " + std::to_string(int(mDamage.get_fire_dmg()))
		+ " W: " + std::to_string(int(mDamage.get_water_dmg()))
		+ " E: " + std::to_string(int(mDamage.get_elec_dmg()));
	mDmg_value->set_text(dmg_value);
	mAs_value->set_text(as_value);
	mRange_value->set_text(range_value);
	mExplosive_radius_value->set_text(std::to_string(int(mExplosive_radius)));
	mDamage_distribution_value->set_text(dmg_distribution_text);

	if (mUpgrade_damage_button->get_state() == L_CLICKABLE_STATE::MOUSE_OVER)
	{
		set_stat_strings_for_upgrade_buttons(mUpgrade_damage_button);
	}
	if (mUpgrade_attackspeed_button->get_state() == L_CLICKABLE_STATE::MOUSE_OVER)
	{
		set_stat_strings_for_upgrade_buttons(mUpgrade_attackspeed_button);
	}

	if (mUpgrade_range_button->get_state() == L_CLICKABLE_STATE::MOUSE_OVER)
	{
		set_stat_strings_for_upgrade_buttons(mUpgrade_range_button);
	}
	Building::update_building_window();
}

void Tower::set_stat_strings_for_upgrade_buttons(UpgradeButton* button)
{
	//updates texture: stat-values for tower
	auto dmg_value = std::to_string(int(mDamage.get_dmg_sum()));
	auto as_value = std::to_string(int(mAttack_speed));
	auto range_value = std::to_string(int(mRange));
	auto explosive_radius_value = std::to_string(int(mExplosive_radius));

	auto tower_upgrade_section = mName + "/upgrade" + button->get_upgrade_section();
	if (button->get_upgrade_section() == "Damage" || button->get_upgrade_section() == "Attackspeed" || button->get_upgrade_section() == "Range")
	{
		tower_upgrade_section = "Tower/upgrade" + button->get_upgrade_section();
	}

	//read upgrade changes out of config
	auto const plus_dmg = gConfig_file->value_or_zero(tower_upgrade_section, "phys") + gConfig_file->value_or_zero(tower_upgrade_section, "magic")
		+ gConfig_file->value_or_zero(tower_upgrade_section, "fire") + gConfig_file->value_or_zero(tower_upgrade_section, "water")
		+ gConfig_file->value_or_zero(tower_upgrade_section, "elec");
	auto const plus_as = gConfig_file->value_or_zero(tower_upgrade_section, "attackspeed");
	auto const plus_range = gConfig_file->value_or_zero(tower_upgrade_section, "range");
	auto const plus_phys_dmg = gConfig_file->value_or_zero(tower_upgrade_section, "phys");
	auto const plus_magic_dmg = gConfig_file->value_or_zero(tower_upgrade_section, "magic");
	auto const plus_fire_dmg = gConfig_file->value_or_zero(tower_upgrade_section, "fire");
	auto const plus_water_dmg = gConfig_file->value_or_zero(tower_upgrade_section, "water");
	auto const plus_elec_dmg = gConfig_file->value_or_zero(tower_upgrade_section, "elec");
	auto const plus_explosive_radius = gConfig_file->value_or_zero(tower_upgrade_section, "explosive_radius");

	//do not change the text if the upgrade doesnt change the value of the stat
	if(plus_dmg != 0) dmg_value += " + " + std::to_string(plus_dmg);
	if(plus_as != 0) as_value += " + " + std::to_string(plus_as);
	if(plus_range != 0)	range_value += " + " + std::to_string(plus_range);
	auto dmg_distribution_text = "P: " + std::to_string(int(mDamage.get_phys_dmg()));
	if (plus_phys_dmg != 0) dmg_distribution_text += " + " + std::to_string(plus_phys_dmg);
	dmg_distribution_text += " M: " + std::to_string(int(mDamage.get_magic_dmg()));
	if (plus_magic_dmg != 0) dmg_distribution_text += " + " + std::to_string(plus_magic_dmg);
	dmg_distribution_text += " F: " + std::to_string(int(mDamage.get_fire_dmg()));
	if (plus_fire_dmg != 0) dmg_distribution_text += " + " + std::to_string(plus_fire_dmg);
	dmg_distribution_text += " W: " + std::to_string(int(mDamage.get_water_dmg()));
	if (plus_water_dmg != 0) dmg_distribution_text += " + " + std::to_string(plus_water_dmg);
	dmg_distribution_text += " E: " + std::to_string(int(mDamage.get_elec_dmg()));
	if (plus_elec_dmg != 0) dmg_distribution_text += " + " + std::to_string(plus_elec_dmg);
	if (plus_explosive_radius != 0) explosive_radius_value += " + " + std::to_string(plus_explosive_radius);

	//set the text changes
	mDmg_value->set_text(dmg_value);
	mAs_value->set_text(as_value);
	mRange_value->set_text(range_value);
	mDamage_distribution_value->set_text(dmg_distribution_text);
	mExplosive_radius_value->set_text(explosive_radius_value);
}

void Tower::on_button_press(const int button_id, Button* button)
{
	if (button_id == UPGRADE_DAMAGE_BUTTON) upgrade_damage();
	if (button_id == UPGRADE_RANGE_BUTTON) upgrade_range();
	if (button_id == UPGRADE_ATTACKSPEED_BUTTON) upgrade_attack_speed();
	Building::on_button_press(button_id, button);
}

void Tower::set_clicked(const bool value)
{
	//break;
	Clickable::set_clicked(value);
}

void Tower::on_tick()
{
	// try to shoot
	const auto all_enemies = gEntity_handler->get_entities_of_type(ENTITYTYPE::ENEMY);
	const auto end = all_enemies->end();
	for (auto it = all_enemies->begin(); it != end; ++it)
	{
		const auto enemy = dynamic_cast<Enemy*>(*it);
		if (enemy_in_range(enemy, mRange, mCoords) && !enemy->is_dead())
		{
			mIdle = !mCurrent_resources->sub_possible(mMaintenance);
			if (mElapsed_ticks % mAttack_cooldown == 0 && !mIdle)
			{
				create_shot(enemy);
				break;
			}

		}
	}
	Building::on_tick();
}

void Tower::on_click(int mouse_x, int mouse_y)
{
	mLevel->get_menu()->set_building_window(mBuilding_window);
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
		mExplosive_radius += gConfig_file->value_or_zero(tower_upgrade_section, "explosive_radius");
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
		mNumber_of_damage_upgrades++;
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
		mNumber_of_range_upgrades++;
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
		mNumber_of_attackspeed_upgrades++;
		return true;
	}
	return false;
}

bool Tower::enemy_in_range(Enemy* enemy, const double radius, const SDL_Point center)
{
	const auto x_div = center.x - enemy->get_position().x - enemy->get_hit_box_offset().x;
	const auto y_div = center.y - enemy->get_position().y - enemy->get_hit_box_offset().y;
	const auto dist_to_enemy = sqrt(x_div * x_div + y_div * y_div);
	return dist_to_enemy <= radius + enemy->get_hitbox_radius();
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

double Tower::get_explosive_radius() const
{
	return mExplosive_radius;
}

