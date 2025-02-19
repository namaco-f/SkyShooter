
#include "../Manager/ResourceManager/Model/ModelResourcesManager.h"
#include "../Manager/ResourceManager/Image/ImageResourcesManager.h"
#include "../Manager/ResourceManager/Shader/ShaderManager.h"
#include "../Manager/ResourceManager/ResourceManager.h"
#include "../Manager/TimeManager.h"

#include "../Object/Common/Collider/Collider.h"
#include "../Object/Common/Collider/Collider2D/Box/Collider2D_BoxXZ.h"
#include "../Renderer/Material/VertexsMaterial.h"
#include "../Renderer/Material/PixelMaterial.h"
#include "../Renderer/Renderer.h"
#include "../Common/Vector2/Vector2f.h"
#include "../Observer/Observer.h"
#include "StageSize.h"
#include "Ground.h"

const VECTOR INIT_POS = { 0.0f,-100.0f,0.0f };		//初期座標
const VECTOR INIT_SCALE = { 200.0f,1.0f,200.0f };	//初期スケール
const Vector2f INIT_UV_SCALE = { 100.0f,100.0f };	//UVスケールの値

const VECTOR INIT_COLIDER2D_CENTERPOS = { 0.0f, 0.0f,0.0f };
const Vector2f INIT_COLIDER2D_SIZE = Vector2f(2000.0f, 2000.0f);

const FLOAT4 GROUND_COLOR = { 0.0f, 0.5f, 1.0f, 1.0f };

Ground::Ground(void)
	:
	ActorBase(),
	uvScrollTime_(0.0f)
{
	objType_ = OBJECT_TYPE::STAGE;
	Init();
}

Ground::~Ground(void)
{
}

void Ground::Update(void)
{
	const auto& deltaTime = TimeManager::GetInstance().GetDeltaTime();

	uvScrollTime_ += deltaTime;

	transform_.Update();
	collider_->UpdateTransform(transform_);
}

void Ground::Draw(void)
{
	auto& uvScroll = resMngShader_.Load(SRC_SHADER::UV_SCROLL);
	const auto& ps = uvScroll.GetPS();
	ps.lock()->SetTextureBuf(0,transform_.modelId_);
	ps.lock()->SetConstBuf(1,FLOAT4(uvScrollTime_, 0.5f, 0.0f, 0.0f));
	
	const auto& render = uvScroll.GetRenderer();

	render.lock()->DrawRenderer([&](void) {MV1DrawModel(transform_.modelId_); });
}

void Ground::InitLoad(void)
{
	resMngModel_.Load(SRC_MODEL::WATERWAVE_CUBE);
}

void Ground::InitTransform(void)
{
	transform_ = Transform();
	transform_.SetModel(resMngModel_.LoadModelDuplicate(SRC_MODEL::WATERWAVE_CUBE));
	transform_.pos = INIT_POS;
	transform_.scl = INIT_SCALE;

	transform_.Update();
}

void Ground::InitCollider(void)
{
	using ColliderData = Collider::ColliderData;
	ColliderData colData = ColliderData();
	colData.transform_ = transform_;
	colData.colType_ = COLLISION_TYPE::MODEL;
	colData.ableCol_ = true;

	collider_ = std::make_unique<Collider>(colData);
	collider_->UpdateCollider(colData);


	ColliderData_2D col2DData = ColliderData_2D();

	col2DData.colCenterPos = INIT_COLIDER2D_CENTERPOS;
	col2DData.size = INIT_COLIDER2D_SIZE;
	collider2D_ = std::make_unique<Collider2D_BoxXZ>(col2DData);
}

void Ground::InitRender(void)
{
	auto& uvScroll = resMngShader_.Load(SRC_SHADER::UV_SCROLL);
	const auto& vs = uvScroll.GetVS();
	vs.lock()->AddConstBuf(FLOAT4(INIT_UV_SCALE.x, INIT_UV_SCALE.y, 1.0f, 1.0f));
	
	const auto& ps = uvScroll.GetPS();

	ps.lock()->SetTextureAddress(PixelMaterial::TEXADDRESS::WRAP);
	ps.lock()->AddTextureBuf(transform_.modelId_);
	ps.lock()->AddConstBuf(GROUND_COLOR);
	ps.lock()->AddConstBuf(FLOAT4(0.0f, 0.0f, 0.0f, 0.0f));

	
	const auto& render = uvScroll.GetRenderer();
}

void Ground::InitEnd(void)
{
}

void Ground::InitIsHitUpdate(void)
{
	//処理なし
}

void Ground::DrawWall(void)
{
	
}
