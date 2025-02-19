
#include <algorithm>
#include "EnemyAttack_Boxes.h"

using namespace std;

EnemyAttack_Boxes::EnemyAttack_Boxes(void)
	:
	enemyAttackBoxes_{},
	boxAttackCnt_(0)
{
}

EnemyAttack_Boxes::~EnemyAttack_Boxes(void)
{
}

void EnemyAttack_Boxes::Update(void)
{
	if (enemyAttackBoxes_.empty())
	{
		//配列が空
		return;
	}

	for (const weak_ptr<EnemyAttack_Box> boxAttack : enemyAttackBoxes_)
	{
		if (boxAttack.expired())
		{
			//箱攻撃が生成されていない
			return;
		}

		//箱攻撃が終了しているか判定
		//終了済 : 終了処理
		//終了していない : 更新処理
		boxAttack.lock()->IsAttackEnd()
			? BoxAttackEnd() : boxAttack.lock()->Update();
	}
}

void EnemyAttack_Boxes::Draw(void)
{
	if (enemyAttackBoxes_.empty())
	{
		//配列が空
		return;
	}

	for (const weak_ptr<EnemyAttack_Box> boxAttack : enemyAttackBoxes_)
	{
		if (boxAttack.expired())
		{
			//箱攻撃が生成されていない
			continue;
		}

		if (boxAttack.lock()->IsAttackEnd())
		{
			//攻撃が終了している
			continue;
		}

		boxAttack.lock()->Draw();
	}
}

const bool EnemyAttack_Boxes::IsBoxesAttackEnd(void) const
{
	if (enemyAttackBoxes_.empty())
	{
		//配列が空
		return true;
	}

	const int boxAttacksNum = (int)enemyAttackBoxes_.size();

	return boxAttacksNum <= boxAttackCnt_;
}

void EnemyAttack_Boxes::BoxesClear(void)
{
	enemyAttackBoxes_.clear();
}

void EnemyAttack_Boxes::BoxAttackEnd(void)
{
	//終了した数を加算
	++boxAttackCnt_;
}
