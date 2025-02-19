
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
		//�z�񂪋�
		return;
	}

	for (const weak_ptr<EnemyAttack_Box> boxAttack : enemyAttackBoxes_)
	{
		if (boxAttack.expired())
		{
			//���U������������Ă��Ȃ�
			return;
		}

		//���U�����I�����Ă��邩����
		//�I���� : �I������
		//�I�����Ă��Ȃ� : �X�V����
		boxAttack.lock()->IsAttackEnd()
			? BoxAttackEnd() : boxAttack.lock()->Update();
	}
}

void EnemyAttack_Boxes::Draw(void)
{
	if (enemyAttackBoxes_.empty())
	{
		//�z�񂪋�
		return;
	}

	for (const weak_ptr<EnemyAttack_Box> boxAttack : enemyAttackBoxes_)
	{
		if (boxAttack.expired())
		{
			//���U������������Ă��Ȃ�
			continue;
		}

		if (boxAttack.lock()->IsAttackEnd())
		{
			//�U�����I�����Ă���
			continue;
		}

		boxAttack.lock()->Draw();
	}
}

const bool EnemyAttack_Boxes::IsBoxesAttackEnd(void) const
{
	if (enemyAttackBoxes_.empty())
	{
		//�z�񂪋�
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
	//�I�������������Z
	++boxAttackCnt_;
}
