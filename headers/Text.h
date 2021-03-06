#pragma once
#include "LTexture.h"
#include "Renderable.h"
#include "Vector.h"
#include "Enums.h"

class Text : public Renderable
{
public:
	Text(const std::string& text, SDL_Rect dim, LAYERS layer, SDL_Color text_color, Renderable* texture_to_render_on = nullptr);
	~Text();

	void add_y_dim(int y);
	void add_x_dim(int x);

	void set_text(const std::string& text);

	void render() override;

	static std::string remove_trailing_zeros(std::string s);

private:
	LTexture* mText_texture = new LTexture();

	std::string mText;

	SDL_Color mText_color{};

	SDL_Rect mDim;

	//if this obj is not rendered then the button is also not rendered (can be null)
	Renderable* mTexture_to_render_on;
};

