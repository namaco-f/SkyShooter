
#include <Dxlib.h>
#include "../../../Application.h"
#include "ModelResource.h"

//確保する配列サイズ
const int SIZE_MAX_DUPLICATE_MODEL_IDS = 10;

ModelResource::ModelResource(const std::string& path)
	:	
	ResourceBase(Application::PATH_MODEL + path),
	duplicateModelIds_{}
{
	if (duplicateModelIds_.empty())
	{
		//一応10個分確保
		duplicateModelIds_.reserve((size_t)SIZE_MAX_DUPLICATE_MODEL_IDS);
	}
}

ModelResource::~ModelResource(void)
{
	duplicateModelIds_.clear();
}

void ModelResource::Load(void)
{
	SetUseASyncLoadFlag(true);
	handleId_ = MV1LoadModel(path_.c_str());
	SetUseASyncLoadFlag(false);
}

void ModelResource::Release(void)
{
	MV1DeleteModel(handleId_);
}

void ModelResource::Add_DuplicateModelId(const int& duplicateHandleId)
{
	if ((int)duplicateModelIds_.size() > SIZE_MAX_DUPLICATE_MODEL_IDS)
	{
		duplicateModelIds_.clear();
	}

	duplicateModelIds_.emplace_back(duplicateHandleId);
	duplicateModelIds_.resize(duplicateModelIds_.size());
}

void ModelResource::Clear_DuplicateModelIds(void)
{
	duplicateModelIds_.clear();
}

const std::vector<int>& ModelResource::Get_DuplicateModelIds(void) const
{
	return duplicateModelIds_;
}

//std::vector<int>& ModelResource::GetDuplicateModelIds(void)
//{
//	return duplicateModelIds_;
//}
