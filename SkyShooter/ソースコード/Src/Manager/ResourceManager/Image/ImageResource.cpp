
#include <Dxlib.h>
#include "../../../Application.h"
#include "ImageResource.h"

ImageResource::ImageResource(const std::string& path) 
	: 
	ResourceBase(Application::PATH_IMAGE + path)
{

}

ImageResource::~ImageResource(void)
{

}

void ImageResource::Load(void)
{
	SetUseASyncLoadFlag(true);
	handleId_ = LoadGraph(path_.c_str());
	SetUseASyncLoadFlag(false);
}

void ImageResource::Release(void)
{

	DeleteGraph(handleId_);

}
