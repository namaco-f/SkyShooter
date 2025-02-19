
#include <Dxlib.h>
#include "../Application.h"
#include "FontResource.h"

FontResource::FontResource(const std::string& path)
	:
	ResourceBase(Application::PATH_FONT + path),
	edgeSize_(0)
{
}

FontResource::~FontResource(void)
{
	Release();
}

void FontResource::Load(void)
{
	handleId_ = LoadFontDataToHandle(path_.c_str(), edgeSize_);
}

void FontResource::Release(void)
{
	DeleteFontToHandle(handleId_);
}

void FontResource::SetEdgeSize(const int edgeSize)
{
	edgeSize_ = edgeSize;
}
