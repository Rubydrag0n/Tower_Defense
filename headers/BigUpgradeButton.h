#pragma once
#include "UpgradeButton.h"

class BigUpgradeButton : public UpgradeButton
{
public:
	BigUpgradeButton(std::string obj_to_upgrade_name, const std::string& button_name, SDL_Rect dim, ButtonObject* obj, std::string upgrade_section, int button_id = 0);
	~BigUpgradeButton();

	void render() override;

private:
	LTexture* mUpgrade_name = new LTexture();
};
