#include "TowerWindow.h"
#include "Tower.h"
#include <iostream>
#include "LayerHandler.h"
#include "UpgradeButton.h"
#include "ConfigFile.h"

TowerWindow::TowerWindow(const SDL_Rect dim, Tower* tower) : BuildingWindow(dim, tower)
{
	SDL_Rect button_dim;
	button_dim.x = mDim.x + 30;
	button_dim.y = mDim.y + 20;
	button_dim.w = 26;
	button_dim.h = 26;
	mUpgrade_damage_button = new UpgradeButton("testbutton", button_dim, this, "Damage", BUILDINGWINDOWBUTTONIDS::UPGRADE_DAMAGE_BUTTON);
	button_dim.x += 56;
	mUpgrade_range_button = new UpgradeButton("testbutton", button_dim, this, "Range", BUILDINGWINDOWBUTTONIDS::UPGRADE_RANGE_BUTTON);
	button_dim.x += 56;
	mUpgrade_attackspeed_button = new UpgradeButton("testbutton", button_dim, this, "Attackspeed", BUILDINGWINDOWBUTTONIDS::UPGRADE_ATTACKSPEED_BUTTON);
	mDmg_text = new LTexture();
	mAs_text = new LTexture();
	mRange_text = new LTexture();
	mDamage_upgrade_number_texture = new LTexture();
	mAttackspeed_upgrade_number_texture = new LTexture();
	mRange_upgrade_number_texture = new LTexture();
	mDamage_distribution_headline = new LTexture();
	mDamage_distribution_headline->load_from_rendered_text("Damagedistribution: ", mText_color);
	mDamage_distribution_text = new LTexture();
}

TowerWindow::~TowerWindow()
{
	delete mUpgrade_damage_button;
	delete mUpgrade_range_button;
	delete mUpgrade_attackspeed_button;
}


void TowerWindow::render()
{
	BuildingWindow::render();
	SDL_Rect dest;
	dest.x = mDim.x + 20;
	dest.y = mDim.y + 260;
	dest.w = 0;	//setting these to 0 will not scale anything
	dest.h = 0;

	auto dmg_text = "Dmg: " + std::to_string(int(dynamic_cast<Tower*>(mBuilding)->get_damage().get_dmg_sum()));
	auto as_text = "AS: " + std::to_string(int(dynamic_cast<Tower*>(mBuilding)->get_attack_speed()));
	auto range_text = "Range: " + std::to_string(int(dynamic_cast<Tower*>(mBuilding)->get_range()));
	auto dmg_distribution_text = "P: " + std::to_string(int(dynamic_cast<Tower*>(mBuilding)->get_damage().get_phys_dmg()))
		+ " M: " + std::to_string(int(dynamic_cast<Tower*>(mBuilding)->get_damage().get_magic_dmg()))
		+ " F: " + std::to_string(int(dynamic_cast<Tower*>(mBuilding)->get_damage().get_fire_dmg()))
		+ " W: " + std::to_string(int(dynamic_cast<Tower*>(mBuilding)->get_damage().get_water_dmg()))
		+ " E: " + std::to_string(int(dynamic_cast<Tower*>(mBuilding)->get_damage().get_elec_dmg()));
	
	//changes string if a upgradebutton is hovered
	for (auto& button : mUpgrade_buttons)
	{
		if (button->get_state() == L_CLICKABLE_STATE::MOUSE_OVER)
			set_stat_strings_for_upgrade_buttons(button, &dmg_text, &as_text, &range_text, &dmg_distribution_text);	
	}
	if (mUpgrade_damage_button->get_state() == L_CLICKABLE_STATE::MOUSE_OVER)
		set_stat_strings_for_upgrade_buttons(mUpgrade_damage_button, &dmg_text, &as_text, &range_text, &dmg_distribution_text);
	if (mUpgrade_attackspeed_button->get_state() == L_CLICKABLE_STATE::MOUSE_OVER)
		set_stat_strings_for_upgrade_buttons(mUpgrade_attackspeed_button, &dmg_text, &as_text, &range_text, &dmg_distribution_text);
	if (mUpgrade_range_button->get_state() == L_CLICKABLE_STATE::MOUSE_OVER)
		set_stat_strings_for_upgrade_buttons(mUpgrade_range_button, &dmg_text, &as_text, &range_text, &dmg_distribution_text);

	mDmg_text->load_from_rendered_text(dmg_text, mText_color);
	gLayer_handler->render_to_layer(mDmg_text, WINDOWS, nullptr, &dest);

	dest.y += 30;
	mAs_text->load_from_rendered_text(as_text, mText_color);
	gLayer_handler->render_to_layer(mAs_text, WINDOWS, nullptr, &dest);

	dest.y += 30;
	mRange_text->load_from_rendered_text(range_text, mText_color);
	gLayer_handler->render_to_layer(mRange_text, WINDOWS, nullptr, &dest);

	dest.y += 40;
	gLayer_handler->render_to_layer(mDamage_distribution_headline, WINDOWS, nullptr, &dest);

	//displays number of little upgrades
	dest.y += 30;
	mDamage_distribution_text->load_from_rendered_text(dmg_distribution_text, mText_color);
	gLayer_handler->render_to_layer(mDamage_distribution_text, WINDOWS, nullptr, &dest);	

	dest.x = mUpgrade_damage_button->get_dimension().x;
	dest.y = mUpgrade_damage_button->get_dimension().y + 30;
	mDamage_upgrade_number_texture->load_from_rendered_text(std::to_string(mNumber_of_damage_upgrades), mText_color);
	gLayer_handler->render_to_layer(mDamage_upgrade_number_texture, WINDOWS, nullptr, &dest);

	dest.x = mUpgrade_attackspeed_button->get_dimension().x;
	dest.y = mUpgrade_attackspeed_button->get_dimension().y + 30;
	mAttackspeed_upgrade_number_texture->load_from_rendered_text(std::to_string(mNumber_of_attackspeed_upgrades), mText_color);
	gLayer_handler->render_to_layer(mAttackspeed_upgrade_number_texture, WINDOWS, nullptr, &dest);

	dest.x = mUpgrade_range_button->get_dimension().x;
	dest.y = mUpgrade_range_button->get_dimension().y + 30;
	mRange_upgrade_number_texture->load_from_rendered_text(std::to_string(mNumber_of_range_upgrades), mText_color);
	gLayer_handler->render_to_layer(mRange_upgrade_number_texture, WINDOWS, nullptr, &dest);
}

void TowerWindow::set_stat_strings_for_upgrade_buttons(Button* button, std::string* dmg_text, std::string* as_text, std::string* range_text, std::string* dmg_distribution_text) const
{
	const auto tower_upgrade_section = mBuilding->get_name() + "/upgrade" + dynamic_cast<UpgradeButton*>(button)->get_upgrade_section();
	*dmg_text += " + " + std::to_string(gConfig_file->value_or_zero(tower_upgrade_section, "phys")
		+ gConfig_file->value_or_zero(tower_upgrade_section, "magic")
		+ gConfig_file->value_or_zero(tower_upgrade_section, "fire")
		+ gConfig_file->value_or_zero(tower_upgrade_section, "water")
		+ gConfig_file->value_or_zero(tower_upgrade_section, "elec"));
	*as_text += " + " + std::to_string(gConfig_file->value_or_zero(tower_upgrade_section, "attackspeed"));
	*range_text += " + " + std::to_string(gConfig_file->value_or_zero(tower_upgrade_section, "range"));
	*dmg_distribution_text = "P: " + std::to_string(int(dynamic_cast<Tower*>(mBuilding)->get_damage().get_phys_dmg())) + " + " + std::to_string(gConfig_file->value_or_zero(tower_upgrade_section, "phys"))
		+ " M: " + std::to_string(int(dynamic_cast<Tower*>(mBuilding)->get_damage().get_magic_dmg())) + " + " + std::to_string(gConfig_file->value_or_zero(tower_upgrade_section, "magic"))
		+ " F: " + std::to_string(int(dynamic_cast<Tower*>(mBuilding)->get_damage().get_fire_dmg())) + " + " + std::to_string(gConfig_file->value_or_zero(tower_upgrade_section, "fire"))
		+ " W: " + std::to_string(int(dynamic_cast<Tower*>(mBuilding)->get_damage().get_water_dmg())) + " + " + std::to_string(gConfig_file->value_or_zero(tower_upgrade_section, "water"))
		+ " E: " + std::to_string(int(dynamic_cast<Tower*>(mBuilding)->get_damage().get_elec_dmg())) + " + " + std::to_string(gConfig_file->value_or_zero(tower_upgrade_section, "elec"));
}


void TowerWindow::upgrade_damage() 
{
	std::cout << "dmg";
	mNumber_of_damage_upgrades++;
	dynamic_cast<Tower*>(mBuilding)->upgrade_damage();
}

void TowerWindow::upgrade_range() 
{
	std::cout << "range";
	mNumber_of_range_upgrades++;
	dynamic_cast<Tower*>(mBuilding)->upgrade_range();
}

void TowerWindow::upgrade_attackspeed() 
{
	std::cout << "speed";
	mNumber_of_attackspeed_upgrades++;
	dynamic_cast<Tower*>(mBuilding)->upgrade_attack_speed();
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

