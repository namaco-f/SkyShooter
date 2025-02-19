#include "ResourceBase.h"

ResourceBase::ResourceBase(const std::string& path)
	:
	handleId_(-1),
	path_(path),
	useSceneIds_{}
{
	//useSceneIds_.reserve((size_t)((int)SCENE_ID::MAX));
	//useSceneIds_.resize(useSceneIds_.size());
}

ResourceBase::~ResourceBase(void)
{
}

const int& ResourceBase::GetHandleId(void) const
{
	return handleId_;
}

const bool ResourceBase::IsUseScene(const SCENE_ID scnId) const
{
	if (useSceneIds_.empty())
	{
		//使用シーン情報が存在しない
		return false;
	}

	for (const SCENE_ID& useSceneId : useSceneIds_)
	{
		if (useSceneId != scnId)
		{
			//使用するシーンではない
			continue;
		}
		
		//使用する
		return true;
	}

	return false;
}
