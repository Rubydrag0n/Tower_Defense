#include "RenderableHandler.h"
#include "Renderable.h"

RenderableHandler* gRenderables_handler = nullptr;

RenderableHandler::RenderableHandler() = default;

RenderableHandler::~RenderableHandler()
{
	mRenderables.clear();
}

void RenderableHandler::add_renderable(Renderable* r)
{
	this->mRenderables.push_back(r);
}

void RenderableHandler::del_renderable(Renderable* r)
{
	for (auto it = mRenderables.begin(); it != mRenderables.end(); ++it)
	{
		if (*it == r)
		{
			mRenderables.erase(it);
			break;
		}
	}
}

void RenderableHandler::render()
{
	for (auto const& it: mRenderables)
	{
		if (it->is_rendering_enabled()) {
			it->render();
		}
	}
}
