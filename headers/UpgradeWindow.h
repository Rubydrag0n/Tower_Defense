#pragma once
#include "Window.h"
#include "Text.h"
#include "Resources.h"

class UpgradeWindow : public Window
{
public:
	UpgradeWindow(SDL_Rect dim, std::string building_upgrade_section);
	~UpgradeWindow();

	void render() override;

private:
	Text** mUpgrade_cost_values;
	Text** mResource_names;
	Text* mHeadline;
	Resources* mUpgrade_costs;
};
