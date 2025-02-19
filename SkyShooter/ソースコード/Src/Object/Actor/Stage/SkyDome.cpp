
#include "../Manager/ResourceManager/Model/ModelResourcesManager.h"
#include "../Observer/Observer.h"
#include "SkyDome.h"

const VECTOR INIT_SCALE = { 200.0f, 200.0f,200.0f };

SkyDome::SkyDome(void)
	:
	ActorBase()
{
	Init();
}

SkyDome::~SkyDome(void)
{
}

void SkyDome::Update(void)
{
	transform_.Update();
}

void SkyDome::Draw(void)
{
	MV1DrawModel(transform_.modelId_);
}

void SkyDome::InitLoad(void)
{
	resMngModel_.Load(SRC_MODEL::SKYDOME);
}

void SkyDome::InitTransform(void)
{
	transform_ = Transform();
	transform_.SetModel(resMngModel_.LoadModelDuplicate(SRC_MODEL::SKYDOME));
	transform_.scl = INIT_SCALE;

	transform_.Update();
}

void SkyDome::InitCollider(void)
{
}

void SkyDome::InitRender(void)
{
}

void SkyDome::InitEnd(void)
{
}

void SkyDome::InitIsHitUpdate(void)
{
	//èàóùÇ»Çµ
}
