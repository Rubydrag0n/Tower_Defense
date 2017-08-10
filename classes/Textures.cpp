#include "Textures.h"
#include "SDL_setup.h"


Textures::Textures() : mContent{}
{
}


Textures::~Textures()
{
	for (auto iter = mContent.begin(); iter != mContent.end(); ++iter)
	{
		//Getting rid of all the left textures
		SDL_DestroyTexture(iter->second);
	}
	//empty the map itself
	mContent.clear();
}

SDL_Texture* Textures::get_texture(std::string path)
{
	//search for the path as key
	auto it = mContent.find(path);
	if (it != mContent.end())	//if the name is found -> it's already loaded
	{
		return it->second;		//return the actual pointer to the SDL_Texture
	}

	//if not yet loaded -> load now

	auto tex = loadTexture(path);	//load texture
	mContent.insert(std::pair<std::string, SDL_Texture*>(path, tex));	//insert key and value in the map

	return tex;
}

bool Textures::del_texture(std::string path)
{
	auto it = mContent.find(path);
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
