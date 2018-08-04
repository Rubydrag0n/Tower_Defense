#include "Renderable.h"
#include "RenderableHandler.h"

Renderable::Renderable()
{
	gRenderables_handler->add_renderable(this);
}

Renderable::~Renderable()
{
	gRenderables_handler->del_renderable(this);
}
