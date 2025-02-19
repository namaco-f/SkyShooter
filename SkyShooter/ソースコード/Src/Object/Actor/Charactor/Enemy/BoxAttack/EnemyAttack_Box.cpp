
#include "../Manager/TimeManager.h"
#include "../Manager/ResourceManager/Model/ModelResourcesManager.h"
#include "../Manager/ResourceManager/Effect/EffectResourcesManager.h"
#include "../Manager/ResourceManager/Shader/ShaderManager.h"
#include "../Effect/EffectController.h"
#include "../../../../Common/Collider/Collider2D/Box/Collider2D_Box.h" 

#include "../../../../../Renderer/Renderer.h"
#include "../../../../../Renderer/Material/PixelMaterial.h"
#include "../../../../../Renderer/Material/VertexsMaterial.h"

#include "../Observer/Observer.h"

#include "EnemyAttack_Box.h"

using namespace std;

const float MODEL_SCALE_CONVERSION_ONE = 0.01f;		//モデルのサイズを1にするための定数

const FLOAT4 BOX_COLOR_WARNING = { 1.0f,1.0f,0.0f,1.0f };
const FLOAT4 BOX_COLOR_ATTACK = { 1.0f,0.0f,0.0f,1.0f };
const FLOAT4 INIT_FLOAT4 = { 1.0f,1.0f,1.0f,1.0f };

const int BOXATTACK_TIME_CONSTBUFFER_IDX = 2;

EnemyAttack_Box::EnemyAttack_Box(
	const Transform& transform, const Vector2f& boxSize, 
	const BoxAttackData& boxAttackData
)
	:
	ActorBase(),
	boxAttackData_(boxAttackData),
	boxType_(BoxType::MAX)
{
	transform_ = transform;
	boxInfo_.boxCenterPos = transform_.pos;
	boxInfo_.boxSize = boxSize;

	objType_ = OBJECT_TYPE::ENEMY_ATACK_BOX;
}

EnemyAttack_Box::~EnemyAttack_Box(void)
{
}

void EnemyAttack_Box::Update(void)
{
	if (boxAttackData_.IsEndAttackTotalTime())
	{
		//攻撃終了済
		return;
	}

	float deltaTime = TimeManager::GetInstance().GetDeltaTime();

	boxAttackData_.step += deltaTime;
	
	//攻撃開始までのディレイが終わっているか
	boxAttackData_.attackDelayEnd ? 
		UpdateAttackTotalTime() : UpdateAttackDelayTime();

	transform_.scl = GetBoxChangeScale();
	transform_.Update();
}

void EnemyAttack_Box::Draw(void)
{
	auto& boxShader = resMngShader_.Load(SRC_SHADER::BOX_SHADER);

	weak_ptr<PixelMaterial> ps = boxShader.GetPS();
	if (ps.expired())
	{
		return;
	}

	weak_ptr<Renderer> renderer = boxShader.GetRenderer();
	if (renderer.expired())
	{
		return;
	}

	if (!boxAttackData_.attackDelayEnd)
	{
		//攻撃開始までのディレイが終了していない

		//時間を更新
		ps.lock()->SetConstBuf(
			BOXATTACK_TIME_CONSTBUFFER_IDX,
			{boxAttackData_.step,boxAttackData_.attackDelay,1.0f,1.0f }
		);
	}

	//メッシュのバックカリングを無効化
	MV1SetMeshBackCulling(transform_.modelId_, 0, false);
	renderer.lock()->DrawRenderer([&] {MV1DrawModel(transform_.modelId_); });
}

void EnemyAttack_Box::SwitchCollider(void)
{
	collider2D_->SwitchAbleCollider();
}

void EnemyAttack_Box::MakeBoxCollider(void)
{

}

void EnemyAttack_Box::UpdateAttackDelayTime(void)
{
	if (boxAttackData_.IsEndAttackDelayTime())
	{
		//ディレイ終了
		boxAttackData_.step = 0.0f;
		boxAttackData_.attackDelayEnd = true;
		SwitchCollider();
		AttackEffectDraw();
	}
}

void EnemyAttack_Box::UpdateAttackTotalTime(void)
{
	if (boxAttackData_.IsEndAttackTotalTime())
	{
		//攻撃終了
		SwitchCollider();
	}
}

void EnemyAttack_Box::InitLoad(void)
{
	resMngModel_.Load(SRC_MODEL::ENEMY_BOXATTACK_CUBE);
}

void EnemyAttack_Box::InitTransform(void)
{
	transform_.SetModel(resMngModel_.LoadModelDuplicate(SRC_MODEL::ENEMY_BOXATTACK_CUBE));
	transform_.scl = GetBoxScale();
	transform_.Update();
}

void EnemyAttack_Box::InitRender(void)
{
	auto& boxShader = resMngShader_.Load(SRC_SHADER::BOX_SHADER);

	weak_ptr<VertexsMaterial> vs = boxShader.GetVS();
	vs.lock()->AddConstBuf(INIT_FLOAT4);
	vs.lock()->AddConstBuf(INIT_FLOAT4);

	weak_ptr<PixelMaterial> ps = boxShader.GetPS();
	ps.lock()->AddTextureBuf(transform_.modelId_);
	ps.lock()->AddConstBuf(BOX_COLOR_WARNING);
	ps.lock()->AddConstBuf(BOX_COLOR_ATTACK);
	ps.lock()->AddConstBuf(INIT_FLOAT4);
	ps.lock()->SetTextureAddress(PixelMaterial::TEXADDRESS::WRAP);

	boxShader.MakeRenderer();
}

void EnemyAttack_Box::InitIsHitUpdate(void)
{
	//処理なし
}

void EnemyAttack_Box::CalculateBoxPos(void)
{
	Vector2f centerPos = { boxInfo_.boxCenterPos.x,boxInfo_.boxCenterPos.y };

	boxInfo_.boxPosTopLeft = centerPos - boxInfo_.boxSize;
	boxInfo_.boxPosBottomLeft = { centerPos.x - boxInfo_.boxSize.x ,centerPos.y + boxInfo_.boxSize.y };

	boxInfo_.boxPosTopRight = { centerPos.x + boxInfo_.boxSize.x,centerPos.y - boxInfo_.boxSize.y };
	boxInfo_.boxPosBottomRight = centerPos + boxInfo_.boxSize;
}

void EnemyAttack_Box::AttackEffectDraw(void)
{
}

const VECTOR EnemyAttack_Box::RegulateBoxScale(const VECTOR& val)
{
	return VScale(val, MODEL_SCALE_CONVERSION_ONE);
}

const float EnemyAttack_Box::LerpBoxScale(const float& max) const
{
	return lerp(0.0f, max, boxAttackData_.attackDelayEndPar());
}
