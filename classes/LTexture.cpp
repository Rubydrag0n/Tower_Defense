#include "LTexture.h"
#include "SDL_setup.h"
#include <SDL_image.h>
#include <SDL_ttf.h>


LTexture::LTexture()
{
	//Initialize
	mTexture = nullptr;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture()
{
	//Deallocate
	free();
}

bool LTexture::load_from_file(std::string path)
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* new_texture = nullptr;

	//Load image at specified path
	auto loaded_surface = IMG_Load(path.c_str());
	if (loaded_surface == nullptr)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		//Color key image
		SDL_SetColorKey(loaded_surface, SDL_TRUE, SDL_MapRGB(loaded_surface->format, 0, 0xFF, 0xFF));

		//Create texture from surface pixels
		new_texture = SDL_CreateTextureFromSurface(gRenderer, loaded_surface);
		if (new_texture == nullptr)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}
		else
		{
			//Get image dimensions
			mWidth = loaded_surface->w;
			mHeight = loaded_surface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loaded_surface);
	}

	//Return success
	mTexture = new_texture;
	return mTexture != nullptr;
}

#ifdef _SDL_TTF_H
bool LTexture::load_from_rendered_text(std::string textureText, SDL_Color textColor)
{
	//Get rid of preexisting texture
	free();
	//Render text surface
	if (gFont == nullptr)
	{
		throw std::errc::bad_file_descriptor;
	}
	auto text_surface = TTF_RenderText_Solid(gFont, textureText.c_str(), textColor);
	if (text_surface == nullptr)
	{
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else
	{
		//Create texture from surface pixels
		mTexture = SDL_CreateTextureFromSurface(gRenderer, text_surface);
		if (mTexture == nullptr)
		{
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}
		else
		{
			//Get image dimensions
			mWidth = text_surface->w;
			mHeight = text_surface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface(text_surface);
	}

	//Return success
	return mTexture != nullptr;
}
#endif

bool LTexture::load_from_surface(SDL_Surface* surface)
{
	//get rid of old texture
	free();

	this->mTexture = SDL_CreateTextureFromSurface(gRenderer, surface);

	this->mWidth = surface->w;
	this->mHeight = surface->h;

	return mTexture != nullptr;
}

void LTexture::free()
{
	//Free texture if it exists
	if (mTexture != nullptr)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = nullptr;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::set_color(Uint8 red, Uint8 green, Uint8 blue) const
{
	//Modulate texture rgb
	SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void LTexture::set_blend_mode(SDL_BlendMode blending) const
{
	//Set blending function
	SDL_SetTextureBlendMode(mTexture, blending);
}

void LTexture::set_alpha(Uint8 alpha) const
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod(mTexture, alpha);
}

void LTexture::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip) const
{
	//Set rendering space and render to screen
	SDL_Rect render_quad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	if (clip != nullptr)
	{
		render_quad.w = clip->w;
		render_quad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx(gRenderer, mTexture, clip, &render_quad, angle, center, flip);
}

int LTexture::get_width() const
{
	return mWidth;
}

int LTexture::get_height() const
{
	return mHeight;
}
