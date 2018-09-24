#include "Renderable.h"
#include "RenderableHandler.h"

Renderable::Renderable() : mRendering_enabled(true)
{
	gRenderables_handler->add_renderable(this);
}

Renderable::~Renderable()
{
	gRenderables_handler->del_renderable(this);
}

bool Renderable::is_rendering_enabled() const
{
	return this->mRendering_enabled;
}

void Renderable::set_rendering_enabled(bool r)
{
	this->mRendering_enabled = r;
}
