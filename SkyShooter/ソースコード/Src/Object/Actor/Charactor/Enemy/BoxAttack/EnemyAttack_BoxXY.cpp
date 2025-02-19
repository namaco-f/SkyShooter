
#include "../Manager/TimeManager.h"
#include "../Manager/ResourceManager/Model/ModelResourcesManager.h"
#include "../Manager/ResourceManager/Effect/EffectResourcesManager.h"
#include "../Effect/EffectController.h"
#include "../Manager/ResourceManager/Shader/ShaderManager.h"
#include "../../../../Common/Collider/Collider2D/Box/Collider2D_BoxXY.h" 

#include "../../../../../Renderer/Renderer.h"
#include "../../../../../Renderer/Material/PixelMaterial.h"
#include "../../../../../Renderer/Material/VertexsMaterial.h"

#include "../Observer/Observer.h"
#include "../../../Stage/StageSize.h"
#include "EnemyAttack_BoxXY.h"

using namespace std;

const string SPLASH = "SPLASH";

EnemyAttack_BoxXY::EnemyAttack_BoxXY(
	const Transform& transform, 
	const Vector2f& boxSize,
	const BoxAttackData& boxAttackData
)
	:
	EnemyAttack_Box(transform,boxSize,boxAttackData)
{
	boxType_ = BoxType::XY;

	const float boxSizeRate = 1000.0f / boxInfo_.boxSize.x;

	MakeBoxCollider();

	InitEffect();

	Init();
}

EnemyAttack_BoxXY::~EnemyAttack_BoxXY(void)
{
}

void EnemyAttack_BoxXY::InitEffect(void)
{
}

void EnemyAttack_BoxXY::MakeBoxCollider(void)
{
	ColliderData_2D colData;
	colData.colCenterPos = boxInfo_.boxCenterPos;
	colData.size = boxInfo_.boxSize;

	collider2D_ = make_unique<Collider2D_BoxXY>(colData, false);
}

void EnemyAttack_BoxXY::CalculateBoxPos(void)
{
}

const VECTOR EnemyAttack_BoxXY::GetBoxScale(void)
{
	VECTOR ret = { boxInfo_.boxSize.x,boxInfo_.boxSize.y,STAGE_MOVELIMIT_Z };

	return RegulateBoxScale(ret);
}

const VECTOR EnemyAttack_BoxXY::GetBoxChangeScale(void)
{
	VECTOR ret = GetBoxScale();

	if (IsAttackDelayEnd())
	{
		return ret;
	}

	ret.z = LerpBoxScale(ret.z);

	return ret;
}
