#pragma once
#include <list>
#include "EnemyAttack_Box.h"

/// <summary>
/// �G�̔��U�����Ǘ�����N���X
/// </summary>
class EnemyAttack_Boxes
{
public:

	EnemyAttack_Boxes(void);
	virtual ~EnemyAttack_Boxes(void);

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update(void);

	/// <summary>
	/// �`�揈��
	/// </summary>
	void Draw(void);

	/// <summary>
	/// ���U���̔z��擾
	/// </summary>
	/// <returns>enemyAttackBoxes_</returns>
	inline const std::list<std::shared_ptr<EnemyAttack_Box>>& GetEnemyAttackBoxes(void) const
	{
		return enemyAttackBoxes_;
	}

	/// <summary>
	/// ���U������
	/// </summary>
	/// <typeparam name="T">���U���N���X</typeparam>
	/// <param name="centerPos">���S���W</param>
	/// <param name="size">�T�C�Y</param>
	template<class T>
	inline void CreateEnemyAttackBox(
		const VECTOR& centerPos, const Vector2f& size,
		const float attackDelay, const float attackTotalTime
	)
	{
		//�������ꂽ��
		bool needCreate = true;

		//�������ɃJ�E���g���Z�b�g
		boxAttackCnt_ = 0;

		Transform trans = Transform();
		trans.pos = centerPos;
		BoxAttackData attackData = BoxAttackData(attackDelay, attackTotalTime);

		if (!enemyAttackBoxes_.empty())
		{
			for (auto& boxAttack : enemyAttackBoxes_)
			{
				if (boxAttack->IsAttackEnd())
				{
					//�U���I���ς̃��������ė��p
					boxAttack = std::move(std::make_shared<T>(
						trans, size, attackData
					));

					needCreate = false;
					break;
				}
			}
		}

		if (!needCreate)
		{
			//�V�K�����̕K�v���Ȃ�
			return;
		}

		enemyAttackBoxes_.emplace_back(std::move(std::make_shared<T>(
			trans, size, attackData
		)));
	}

	/// <summary>
	/// �U�����I�����Ă��邩
	/// </summary>
	virtual const bool IsBoxesAttackEnd(void) const;

	virtual void BoxesClear(void);

private:

	//���U������
	std::list<std::shared_ptr<EnemyAttack_Box>> enemyAttackBoxes_;

	//�U���I���������U���̃J�E���g
	int boxAttackCnt_;

	//���U���̏I������
	virtual void BoxAttackEnd(void);

};