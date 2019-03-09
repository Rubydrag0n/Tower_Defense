#include "TowerWindow.h"
#include "Tower.h"
#include <iostream>
#include "LayerHandler.h"
#include "UpgradeButton.h"
#include "ConfigFile.h"

TowerWindow::TowerWindow(const SDL_Rect dim, Tower* tower) : BuildingWindow(dim, tower)
{
	SDL_Rect button_dim;
	button_dim.x = mDim.x + 170;
	button_dim.y = mDim.y;
	button_dim.w = 26;
	button_dim.h = 26;
	mUpgrade_damage_button = new Button("testbutton", button_dim, this, BUILDINGWINDOWBUTTONIDS::UPGRADE_DAMAGE_BUTTON);
	button_dim.y += 30;
	mUpgrade_range_button = new Button("testbutton", button_dim, this, BUILDINGWINDOWBUTTONIDS::UPGRADE_RANGE_BUTTON);
	button_dim.y += 30;
	mUpgrade_attackspeed_button = new Button("testbutton", button_dim, this, BUILDINGWINDOWBUTTONIDS::UPGRADE_ATTACKSPEED_BUTTON);
	mDmg_text = new LTexture();
	mAs_text = new LTexture();
	mRange_text = new LTexture();
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
	dest.x = get_dim().x + 10;
	dest.y = get_dim().y + 220;
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

	for (auto& button : mUpgrade_buttons)
	{
		if (button->get_state() == L_CLICKABLE_STATE::MOUSE_OVER)
		{
			const auto tower_upgrade_section = mBuilding->get_name() + "/upgrade" + dynamic_cast<UpgradeButton*>(button)->get_upgrade_section();
			dmg_text += " + " + std::to_string(gConfig_file->value_or_zero(tower_upgrade_section, "phys")
				+ gConfig_file->value_or_zero(tower_upgrade_section, "magic")
				+ gConfig_file->value_or_zero(tower_upgrade_section, "fire")
				+ gConfig_file->value_or_zero(tower_upgrade_section, "water")
				+ gConfig_file->value_or_zero(tower_upgrade_section, "elec"));
			as_text += " + " + std::to_string(gConfig_file->value_or_zero(tower_upgrade_section, "attackspeed"));
			range_text += " + " + std::to_string(gConfig_file->value_or_zero(tower_upgrade_section, "range"));
			dmg_distribution_text = "P: " + std::to_string(int(dynamic_cast<Tower*>(mBuilding)->get_damage().get_phys_dmg())) + " + " + std::to_string(gConfig_file->value_or_zero(tower_upgrade_section, "phys"))
				+ " M: " + std::to_string(int(dynamic_cast<Tower*>(mBuilding)->get_damage().get_magic_dmg())) + " + " + std::to_string(gConfig_file->value_or_zero(tower_upgrade_section, "magic"))
				+ " F: " + std::to_string(int(dynamic_cast<Tower*>(mBuilding)->get_damage().get_fire_dmg())) + " + " + std::to_string(gConfig_file->value_or_zero(tower_upgrade_section, "fire"))
				+ " W: " + std::to_string(int(dynamic_cast<Tower*>(mBuilding)->get_damage().get_water_dmg())) + " + " + std::to_string(gConfig_file->value_or_zero(tower_upgrade_section, "water"))
					+ " E: " + std::to_string(int(dynamic_cast<Tower*>(mBuilding)->get_damage().get_elec_dmg()))  +" + " + std::to_string(gConfig_file->value_or_zero(tower_upgrade_section, "elec"));
		}
	}

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

	dest.y += 30;
	mDamage_distribution_text->load_from_rendered_text(dmg_distribution_text, mText_color);
	gLayer_handler->render_to_layer(mDamage_distribution_text, WINDOWS, nullptr, &dest);	
}

void TowerWindow::upgrade_damage() const
{
	std::cout << "dmg";
	dynamic_cast<Tower*>(mBuilding)->upgrade_damage();
}

void TowerWindow::upgrade_range() const
{
	std::cout << "range";
	dynamic_cast<Tower*>(mBuilding)->upgrade_range();
}

void TowerWindow::upgrade_attackspeed() const
{
	std::cout << "speed";
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

