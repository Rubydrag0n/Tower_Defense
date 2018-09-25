#pragma once

class Renderable
{
public:
	Renderable();
	virtual ~Renderable();

	virtual void render() = 0;

	//returns if the object is currently being rendered
	bool is_rendering_enabled() const;

	//sets if this object is being rendered.
	void set_rendering_enabled(bool r);

private:
	bool mRendering_enabled;
};