#pragma once
#include <map>
#include <SDL.h>


//A class to handle all the textures so they don't get loadet more than once
class Textures
{
public:
	Textures();
	~Textures();

	//retrieving a texture
	//will load the texture if it's not yet saved
	SDL_Texture* get_texture(std::string name);

	//deletes a loaded texture with a given name
	bool del_texture(std::string name);

private:
	//stores the unit name strings and the associated sprite
	std::map<std::string, SDL_Texture*> mContent;
};

