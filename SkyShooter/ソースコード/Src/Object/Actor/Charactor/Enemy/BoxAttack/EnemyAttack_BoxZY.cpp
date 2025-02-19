
#include "../Manager/TimeManager.h"
#include "../../../../Common/Collider/Collider2D/Box/Collider2D_BoxZY.h" 

#include "../../../../../Renderer/Renderer.h"
#include "../../../../../Renderer/Material/PixelMaterial.h"
#include "../../../../../Renderer/Material/VertexsMaterial.h"

#include "../Observer/Observer.h"
#include "../../../Stage/StageSize.h"
#include "EnemyAttack_BoxZY.h"

using namespace std;

const float MODEL_SCALE_CONVERSION_ONE = 0.01f;

EnemyAttack_BoxZY::EnemyAttack_BoxZY(
	const Transform& transform, 
	const Vector2f& boxSize,
	const BoxAttackData& boxAttackData
)
	:
	EnemyAttack_Box(transform, boxSize, boxAttackData)
{
	boxType_ = BoxType::ZY;
	MakeBoxCollider();
	Init();
}

EnemyAttack_BoxZY::~EnemyAttack_BoxZY(void)
{
}

void EnemyAttack_BoxZY::MakeBoxCollider(void)
{
	ColliderData_2D colData;
	colData.colCenterPos = boxInfo_.boxCenterPos;
	colData.size = boxInfo_.boxSize;

	collider2D_ = make_unique<Collider2D_BoxZY>(colData, false);
}

const VECTOR EnemyAttack_BoxZY::GetBoxScale(void)
{
	VECTOR ret = { STAGE_MOVELIMIT_X,boxInfo_.boxSize.y,boxInfo_.boxSize.x };

	return RegulateBoxScale(ret);
}

const VECTOR EnemyAttack_BoxZY::GetBoxChangeScale(void)
{
	VECTOR ret = GetBoxScale();

	if (IsAttackDelayEnd())
	{
		//çUåÇîÕàÕÇëSÇƒï`âÊçœ
		return ret;
	}

	ret.x = LerpBoxScale(ret.x);

	return ret;
}
