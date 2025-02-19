
#include <Dxlib.h>
#include "../../../Application.h"
#include "ImagesResource.h"

ImagesResource::ImagesResource(const std::string& path, const Vector2& num, const Vector2& size)
	: 
	ResourceBase(Application::PATH_IMAGE + path),
	imagesInfo_(ImagesInfo(num, size))
{

}

ImagesResource::~ImagesResource(void)
{

}

void ImagesResource::Load(void)
{

	int numMax = imagesInfo_.GetNumMax();

	imagesInfo_.handleIds = new int[numMax];

	SetUseASyncLoadFlag(true);
	LoadDivGraph(
		path_.c_str(),
		numMax,
		imagesInfo_.num.x, imagesInfo_.num.y,
		imagesInfo_.size.x, imagesInfo_.size.y,
		&imagesInfo_.handleIds[0]
	);
	SetUseASyncLoadFlag(false);
}

void ImagesResource::Release(void)
{

	int numMax = imagesInfo_.GetNumMax();
	for (int i = 0; i < numMax; i++)
	{
		DeleteGraph(imagesInfo_.handleIds[i]);
	}

	delete[] imagesInfo_.handleIds;

}

void ImagesResource::CopyImages(int* imgs)
{

	if (imagesInfo_.handleIds == nullptr)
	{
		//”z—ñ‚ª‹ó‚ÌŽž‚Íˆ—‚µ‚È‚¢
		return;
	}

	int numMax = imagesInfo_.GetNumMax();
	for (int i = 0; i < numMax; i++)
	{
		imgs[i] = imagesInfo_.handleIds[i];
	}

}

const ImagesResource::ImagesInfo& ImagesResource::GetImagesInfo(void) const
{
	return imagesInfo_;
}
