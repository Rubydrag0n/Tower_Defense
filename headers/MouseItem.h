#pragma once
#include "Clickable.h"
#include "LTexture.h"
#include "Resources.h"
#include "Level.h"

class Level;

class MouseItem final : public Clickable
{
public:
	MouseItem(const std::string& name_of_object, LTexture* sprite, Level* level, const Resources& construction_costs);

	void set_name_of_object(const std::string& name);
	std::string get_name_of_object() const;

	void on_click(int mouse_x, int mouse_y) override;
	void on_right_click(int mouse_x, int mouse_y) override;

	void render() const;

private:
	std::string mName_of_object;
	LTexture* mSprite;
	Level* mLevel;
	Resources mConstruction_costs;
	std::string mGrid_sprite_path;
	LTexture* mGrid_sprite;
	TILETYPES mTile_to_build_on;

};
