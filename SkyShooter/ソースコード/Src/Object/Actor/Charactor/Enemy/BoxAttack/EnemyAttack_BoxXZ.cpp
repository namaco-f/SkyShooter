
#include "../Manager/TimeManager.h"
#include "../Manager/ResourceManager/Model/ModelResourcesManager.h"
#include "../../../../Common/Collider/Collider2D/Box/Collider2D_BoxXZ.h" 
#include "../Manager/ResourceManager/Effect/EffectResourcesManager.h"
#include "../Effect/EffectController.h"
#include "../../../../../Renderer/Renderer.h"
#include "../../../../../Renderer/Material/PixelMaterial.h"
#include "../../../../../Renderer/Material/VertexsMaterial.h"

#include "../Observer/Observer.h"
#include "../../../Stage/StageSize.h"
#include "EnemyAttack_BoxXZ.h"

using namespace std;

const string SPLASH = "SPLASH";

EnemyAttack_BoxXZ::EnemyAttack_BoxXZ(
	const Transform& transform,
	const Vector2f& boxSize,
	const BoxAttackData& boxAttackData
)
	:
	EnemyAttack_Box(transform, boxSize, boxAttackData)
{
	boxType_ = BoxType::XZ;
	MakeBoxCollider();
	Init();
	InitEffect();
}

EnemyAttack_BoxXZ::~EnemyAttack_BoxXZ(void)
{
}

void EnemyAttack_BoxXZ::InitEffect(void)
{
}

void EnemyAttack_BoxXZ::AttackEffectDraw(void)
{
}

void EnemyAttack_BoxXZ::MakeBoxCollider(void)
{
	ColliderData_2D colData;
	colData.colCenterPos = boxInfo_.boxCenterPos;
	colData.size = boxInfo_.boxSize;

	collider2D_ = make_unique<Collider2D_BoxXZ>(colData,false);
}

const VECTOR EnemyAttack_BoxXZ::GetBoxScale(void)
{
	VECTOR ret = { boxInfo_.boxSize.x,STAGE_MOVELIMIT_Y,boxInfo_.boxSize.y };

	return RegulateBoxScale(ret);
}

const VECTOR EnemyAttack_BoxXZ::GetBoxChangeScale(void)
{
	VECTOR ret = GetBoxScale();

	if (IsAttackDelayEnd())
	{
		return ret;
	}

	ret.y = LerpBoxScale(ret.y);

	return ret;
}
