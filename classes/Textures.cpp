#include "Textures.h"
#include "SDL_setup.h"
#include "ConfigFile.h"


Textures::Textures() : mContent{}
{
}


Textures::~Textures()
{
	mContent.clear();
}

SDL_Texture* Textures::get_texture(std::string name)
{
	ConfigFile cf("config/game.cfg");
	//load the path from the config file
	std::string path = cf.Value(name + "/sprite", "path");

	//search for the name as key
	auto it = mContent.find(name);
	if (it != mContent.end())	//if the name is found -> it's already loaded
	{
		return it->second;		//return the actual pointer to the SDL_Texture
	}

	//if not yet loaded -> load now

	auto tex = loadTexture(path);	//load texture
	mContent.insert(std::pair<std::string, SDL_Texture*>(name, tex));	//insert key and value in the map

	return tex;
}

bool Textures::del_texture(std::string name)
{
	auto it = mContent.find(name);
	if (it != mContent.end())
	{
		//deallocate memory
		SDL_DestroyTexture(it->second);
		//destroy the entry in the map
		mContent.erase(it);
		return true;
	}

	// if it's not yet in the map, it can't be deleted
	return false;
}
