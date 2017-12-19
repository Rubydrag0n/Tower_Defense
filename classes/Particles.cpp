#include "Particles.h"
#include "ConfigFile.h"
#include "SDL_setup.h"

Particles::Particles(std::string particle_name, CoordinatesInDouble pos, CoordinatesInDouble dir, float rot) : mRotation_speed{rot}, mPosition{pos}, mSpeed_and_direction{dir}
{
	mTexture = gTextures->get_texture(gConfig_file->Value("animation/" + particle_name, "path"));
	std::string fliptext = gConfig_file->Value("animation/" + particle_name, "flip");
	if (fliptext == "none")
	{
		mFlip = SDL_FLIP_NONE;
	} else if (fliptext == "vertical")
	{
		mFlip = SDL_FLIP_VERTICAL;
	} else if (fliptext == "horizontal")
	{
		mFlip = SDL_FLIP_HORIZONTAL;
	} else
	{
		mFlip = SDL_FLIP_NONE;
	}
	mLife_ticks = gConfig_file->Value("animation/" + particle_name, "lifespan");
	mTick = 0;
}

Particles::~Particles()
{
}

void Particles::update()
{

}

bool Particles::delete_me()
{
	return false;
}
