#include "RenderableHandler.h"
#include "Renderable.h"

RenderableHandler* gRenderables_handler = nullptr;

RenderableHandler::RenderableHandler() : mRenderables()
{

}

RenderableHandler::~RenderableHandler()
{
	mRenderables.empty();
}

void RenderableHandler::add_renderable(Renderable* r)
{
	this->mRenderables.push_back(r);
}

void RenderableHandler::del_renderable(Renderable* r)
{
	for (auto it = mRenderables.begin(); it != mRenderables.end(); it++)
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
	for (auto it = this->mRenderables.begin(); it != mRenderables.end(); it++)
	{
		(*it)->render();
	}
}
