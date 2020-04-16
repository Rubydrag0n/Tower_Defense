#include <filesystem>
#include <fstream>
#include <sstream>
#include <google/protobuf/util/json_util.h>

#include "LevelSelectMenu.h"
#include "ConfigFile.h"
#include "SDL_setup.h"
#include "Button.h"
#include "Game.h"

LevelSelectMenu::LevelSelectMenu(Game* game) : MainMenuTab(game)
{
	const auto sprite_section = "LevelSelectMenu/sprite";

	mBackground_sprite = gTextures->get_texture(gConfig_file->value(sprite_section, "background"));

	SDL_Rect dim;
	dim.w = gConfig_file->value("button/play", "clip_width");
	dim.h = gConfig_file->value("button/play", "clip_height");
	//start location and distance between each button, which can be clicked to start the individual levels
	const auto first_button_x_location = 100;
	const auto first_button_y_location = 100;
	const auto distance_between_buttons_x = 400;
	const auto distance_between_buttons_y = 200;
	dim.x = first_button_x_location;
	dim.y = first_button_y_location;

	/*for(auto i = 1; gConfig_file->value_exists("level" + std::to_string(i), "exists"); ++i)
	{
		auto new_level_button = new Button("play", dim, this, OVERLAY, OVERLAY, i);
		mButtons.push_back(new_level_button);
		dim.x += distance_between_buttons_x;
		if (dim.x > LOGICAL_SCREEN_WIDTH)
		{
			dim.x = first_button_x_location;
			dim.y += distance_between_buttons_y;
		}
	}*/

	auto i = 0;
	for (const auto& p : std::filesystem::directory_iterator("level"))
	{
		std::ifstream file(p.path());
		if (!file.is_open())
		{
			std::cerr << "Couldn't open map at " << p.path() << std::endl;
		}
		std::stringstream json_string;

		json_string << file.rdbuf();

		towerdefense::map check_valid;
		auto status = google::protobuf::util::JsonStringToMessage(json_string.str(), &check_valid);

		if (status.ok())
		{
			auto new_level_button = new Button("play", dim, this, OVERLAY, OVERLAY, ++i);
			mButtons.push_back(new_level_button);
			dim.x += distance_between_buttons_x;
			if (dim.x > LOGICAL_SCREEN_WIDTH)
			{
				dim.x = first_button_x_location;
				dim.y += distance_between_buttons_y;
			}
		}
		else {
			std::cerr << "Couldn't load map at " << p.path() << std::endl;
		}
	}
	this->set_enabled(false);
}

void LevelSelectMenu::on_button_press(const int button_id, Button* button)
{
	this->set_enabled(false);
	mGame->load_level(button_id);
	mGame->set_state(Game::STATE::PLAYING);
}
