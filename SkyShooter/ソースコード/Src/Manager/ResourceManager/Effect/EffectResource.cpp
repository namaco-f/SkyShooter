#include <EffekseerForDXLib.h>
#include "../../../Application.h"
#include "EffectResource.h"

EffectResource::EffectResource(const std::string& path) 
	:
	ResourceBase(Application::PATH_EFFECT + path)
{
}

EffectResource::~EffectResource(void)
{
}

void EffectResource::Load(void)
{
	//SetUseASyncLoadFlag(true);
	handleId_ = LoadEffekseerEffect(path_.c_str());
	//SetUseASyncLoadFlag(false);
}

void EffectResource::Release(void)
{
	//Dxlib_End()�O�ɉ���̕K�v�L
	DeleteEffekseerEffect(handleId_);
}
