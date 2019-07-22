#include "TowerWindow.h"
#include "Tower.h"
#include <iostream>
#include "LayerHandler.h"
#include "UpgradeButton.h"
#include "ConfigFile.h"
#include "BigUpgrade.h"

TowerWindow::TowerWindow(const SDL_Rect dim, Tower* tower) : BuildingWindow(dim, tower)
{
	mBuilding = tower;
	
	SDL_Rect button_dim;
	button_dim.x = mDim.x + 30;
	button_dim.y = mDim.y + 140;
	button_dim.w = 26;
	button_dim.h = 26;
	mUpgrade_damage_button = new UpgradeButton("testbutton", button_dim, this, "Damage", WINDOWCONTENT, WINDOWCONTENT, this, BUILDINGWINDOWBUTTONIDS::UPGRADE_DAMAGE_BUTTON);
	button_dim.x += 56;
	mUpgrade_range_button = new UpgradeButton("testbutton", button_dim, this, "Range", WINDOWCONTENT, WINDOWCONTENT, this, BUILDINGWINDOWBUTTONIDS::UPGRADE_RANGE_BUTTON);
	button_dim.x += 56;
	mUpgrade_attackspeed_button = new UpgradeButton("testbutton", button_dim, this, "Attackspeed", WINDOWCONTENT, WINDOWCONTENT, this, BUILDINGWINDOWBUTTONIDS::UPGRADE_ATTACKSPEED_BUTTON);
	SDL_Rect dest;
	dest.h = 0;
	dest.w = 0;
	//number of little upgrades displayed
	dest.x = mUpgrade_damage_button->get_dimension().x;
	dest.y = mUpgrade_damage_button->get_dimension().y + 30;
	mDamage_upgrade_number_texture = new Text(std::to_string(mBuilding->get_number_of_damage_upgrades()), dest, WINDOWS, mText_color, this);

	dest.x = mUpgrade_attackspeed_button->get_dimension().x;
	dest.y = mUpgrade_attackspeed_button->get_dimension().y + 30;
	mAttackspeed_upgrade_number_texture = new Text(std::to_string(mBuilding->get_number_of_attackspeed_upgrades()), dest, WINDOWS, mText_color, this);

	dest.x = mUpgrade_range_button->get_dimension().x;
	dest.y = mUpgrade_range_button->get_dimension().y + 30;
	mRange_upgrade_number_texture = new Text(std::to_string(mBuilding->get_number_of_range_upgrades()), dest, WINDOWS, mText_color, this);

	//turret stats-text displayed(const)
	dest.x = mDim.x + 200;
	dest.y = mDim.y + 20;
	dest.w = 0;	//setting these to 0 will not scale anything
	dest.h = 0;
	mDmg_text = new Text("Dmg: ", dest, WINDOWS, mText_color, this);
	dest.y += 30;
	mAs_text = new Text("AS: ", dest, WINDOWS, mText_color, this);
	dest.y += 30;
	mRange_text = new Text("Range: ", dest, WINDOWS, mText_color, this);
	dest.y += 30;
	mDamage_distribution_headline = new Text("Damage dist: ", dest, WINDOWS, mText_color, this);
	dest.y += 30;
	mDamage_distribution_text = new Text("P: " + std::to_string(int(dynamic_cast<Tower*>(mBuilding)->get_damage().get_phys_dmg()))
		+ " M: " + std::to_string(int(dynamic_cast<Tower*>(mBuilding)->get_damage().get_magic_dmg()))
		+ " F: " + std::to_string(int(dynamic_cast<Tower*>(mBuilding)->get_damage().get_fire_dmg()))
		+ " W: " + std::to_string(int(dynamic_cast<Tower*>(mBuilding)->get_damage().get_water_dmg()))
		+ " E: " + std::to_string(int(dynamic_cast<Tower*>(mBuilding)->get_damage().get_elec_dmg())), dest, WINDOWS, mText_color, this);

	//turret stats-numbers displayed(dynamic)
	dest.x = mDim.x + 260;
	dest.y = mDim.y + 20;
	mDmg_value = new Text(std::to_string(int(dynamic_cast<Tower*>(mBuilding)->get_damage().get_dmg_sum())), dest, WINDOWS, mText_color, this);
	dest.y += 30;
	mAs_value = new Text(std::to_string(int(dynamic_cast<Tower*>(mBuilding)->get_attack_speed())), dest, WINDOWS, mText_color, this);
	dest.y += 30;
	mRange_value = new Text(std::to_string(int(dynamic_cast<Tower*>(mBuilding)->get_range())), dest, WINDOWS, mText_color, this);
}

TowerWindow::~TowerWindow()
{
	delete mUpgrade_damage_button;
	delete mUpgrade_range_button;
	delete mUpgrade_attackspeed_button;
	delete mDmg_text;
	delete mDmg_value;
	delete mAs_text;
	delete mAs_value;
	delete mRange_text;
	delete mRange_value;
	delete mDamage_distribution_headline;
	delete mDamage_distribution_text;
	delete mDamage_upgrade_number_texture;
	delete mAttackspeed_upgrade_number_texture;
	delete mRange_upgrade_number_texture;
}

void TowerWindow::render()
{
	BuildingWindow::render();

	//updates texture: number of little upgrades
	mDamage_upgrade_number_texture->set_text(std::to_string(mBuilding->get_number_of_damage_upgrades()));
	mAttackspeed_upgrade_number_texture->set_text(std::to_string(mBuilding->get_number_of_attackspeed_upgrades()));
	mRange_upgrade_number_texture->set_text(std::to_string(mBuilding->get_number_of_range_upgrades()));

	//changes string if a upgradebutton is hovered
	auto is_a_button_hovered = false;
	for (auto& upgrade : mBig_upgrades)
	{
		if (upgrade->get_big_upgrade_button()->get_state() == L_CLICKABLE_STATE::MOUSE_OVER)
		{
			set_stat_strings_for_upgrade_buttons(upgrade->get_big_upgrade_button());
			is_a_button_hovered = true;
		}
	}
	if (mUpgrade_damage_button->get_state() == L_CLICKABLE_STATE::MOUSE_OVER)
	{
		set_stat_strings_for_upgrade_buttons(mUpgrade_damage_button);
		is_a_button_hovered = true;
	}
	if (mUpgrade_attackspeed_button->get_state() == L_CLICKABLE_STATE::MOUSE_OVER)
	{
		set_stat_strings_for_upgrade_buttons(mUpgrade_attackspeed_button);
		is_a_button_hovered = true;
	}

	if (mUpgrade_range_button->get_state() == L_CLICKABLE_STATE::MOUSE_OVER)
	{
		set_stat_strings_for_upgrade_buttons(mUpgrade_range_button);
		is_a_button_hovered = true;
	}

	if(!is_a_button_hovered)
	{
		set_stat_strings_to_normal();
	}
}

void TowerWindow::set_stat_strings_to_normal()
{
	auto dmg_value = std::to_string(int(dynamic_cast<Tower*>(mBuilding)->get_damage().get_dmg_sum()));
	auto as_value = std::to_string(int(dynamic_cast<Tower*>(mBuilding)->get_attack_speed()));
	auto range_value = std::to_string(int(dynamic_cast<Tower*>(mBuilding)->get_range()));
	auto dmg_distribution_text = "P: " + std::to_string(int(dynamic_cast<Tower*>(mBuilding)->get_damage().get_phys_dmg()))
		+ " M: " + std::to_string(int(dynamic_cast<Tower*>(mBuilding)->get_damage().get_magic_dmg()))
		+ " F: " + std::to_string(int(dynamic_cast<Tower*>(mBuilding)->get_damage().get_fire_dmg()))
		+ " W: " + std::to_string(int(dynamic_cast<Tower*>(mBuilding)->get_damage().get_water_dmg()))
		+ " E: " + std::to_string(int(dynamic_cast<Tower*>(mBuilding)->get_damage().get_elec_dmg()));
	mDmg_value->set_text(dmg_value);
	mAs_value->set_text(as_value);
	mRange_value->set_text(range_value);
	mDamage_distribution_text->set_text(dmg_distribution_text);
}


void TowerWindow::set_stat_strings_for_upgrade_buttons(Button* button)
{
	//updates texture: stat-values for tower
	auto dmg_value = std::to_string(int(dynamic_cast<Tower*>(mBuilding)->get_damage().get_dmg_sum()));
	auto as_value = std::to_string(int(dynamic_cast<Tower*>(mBuilding)->get_attack_speed()));
	auto range_value = std::to_string(int(dynamic_cast<Tower*>(mBuilding)->get_range()));
	
	auto tower_upgrade_section = mBuilding->get_name() + "/upgrade" + dynamic_cast<UpgradeButton*>(button)->get_upgrade_section();
	if(dynamic_cast<UpgradeButton*>(button)->get_upgrade_section() == "Damage" || dynamic_cast<UpgradeButton*>(button)->get_upgrade_section() == "Attackspeed" || dynamic_cast<UpgradeButton*>(button)->get_upgrade_section() == "Range")
	{
		tower_upgrade_section = "Tower/upgrade" + dynamic_cast<UpgradeButton*>(button)->get_upgrade_section();
	}

	dmg_value += " + " + std::to_string(gConfig_file->value_or_zero(tower_upgrade_section, "phys")
		+ gConfig_file->value_or_zero(tower_upgrade_section, "magic")
		+ gConfig_file->value_or_zero(tower_upgrade_section, "fire")
		+ gConfig_file->value_or_zero(tower_upgrade_section, "water")
		+ gConfig_file->value_or_zero(tower_upgrade_section, "elec"));
	as_value += " + " + std::to_string(gConfig_file->value_or_zero(tower_upgrade_section, "attackspeed"));
	range_value += " + " + std::to_string(gConfig_file->value_or_zero(tower_upgrade_section, "range"));
	auto dmg_distribution_text = "P: " + std::to_string(int(dynamic_cast<Tower*>(mBuilding)->get_damage().get_phys_dmg())) + " + " + std::to_string(gConfig_file->value_or_zero(tower_upgrade_section, "phys"))
		+ " M: " + std::to_string(int(dynamic_cast<Tower*>(mBuilding)->get_damage().get_magic_dmg())) + " + " + std::to_string(gConfig_file->value_or_zero(tower_upgrade_section, "magic"))
		+ " F: " + std::to_string(int(dynamic_cast<Tower*>(mBuilding)->get_damage().get_fire_dmg())) + " + " + std::to_string(gConfig_file->value_or_zero(tower_upgrade_section, "fire"))
		+ " W: " + std::to_string(int(dynamic_cast<Tower*>(mBuilding)->get_damage().get_water_dmg())) + " + " + std::to_string(gConfig_file->value_or_zero(tower_upgrade_section, "water"))
		+ " E: " + std::to_string(int(dynamic_cast<Tower*>(mBuilding)->get_damage().get_elec_dmg())) + " + " + std::to_string(gConfig_file->value_or_zero(tower_upgrade_section, "elec"));

	mDmg_value->set_text(dmg_value);
	mAs_value->set_text(as_value);
	mRange_value->set_text(range_value);
	mDamage_distribution_text->set_text(dmg_distribution_text);
}

void TowerWindow::upgrade_damage() 
{
	if(mBuilding->upgrade_damage()) mBuilding->increment_number_of_damage_upgrades();
}

void TowerWindow::upgrade_range() 
{
	if(mBuilding->upgrade_range()) mBuilding->increment_number_of_range_upgrades();
}

void TowerWindow::upgrade_attackspeed() 
{
	if(mBuilding->upgrade_attack_speed()) mBuilding->increment_number_of_attackspeed_upgrades();
}

void TowerWindow::on_button_press(const int button_id, Button* button)
{
	if (button_id == UPGRADE_DAMAGE_BUTTON) this->upgrade_damage();
	if (button_id == UPGRADE_RANGE_BUTTON) this->upgrade_range();
	if (button_id == UPGRADE_ATTACKSPEED_BUTTON) this->upgrade_attackspeed();
	BuildingWindow::on_button_press(button_id, button);
}

Button* TowerWindow::get_upgrade_damage_button() const
{
	return mUpgrade_damage_button;
}

Button* TowerWindow::get_upgrade_range_button() const
{
	return mUpgrade_range_button;
}

Button* TowerWindow::get_upgrade_attackspeed_button() const
{
	return mUpgrade_attackspeed_button;
}
