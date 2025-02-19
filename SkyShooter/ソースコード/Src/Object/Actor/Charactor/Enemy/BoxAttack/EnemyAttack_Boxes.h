#pragma once
#include <list>
#include "EnemyAttack_Box.h"

/// <summary>
/// 敵の箱攻撃を管理するクラス
/// </summary>
class EnemyAttack_Boxes
{
public:

	EnemyAttack_Boxes(void);
	virtual ~EnemyAttack_Boxes(void);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(void);

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(void);

	/// <summary>
	/// 箱攻撃の配列取得
	/// </summary>
	/// <returns>enemyAttackBoxes_</returns>
	inline const std::list<std::shared_ptr<EnemyAttack_Box>>& GetEnemyAttackBoxes(void) const
	{
		return enemyAttackBoxes_;
	}

	/// <summary>
	/// 箱攻撃生成
	/// </summary>
	/// <typeparam name="T">箱攻撃クラス</typeparam>
	/// <param name="centerPos">中心座標</param>
	/// <param name="size">サイズ</param>
	template<class T>
	inline void CreateEnemyAttackBox(
		const VECTOR& centerPos, const Vector2f& size,
		const float attackDelay, const float attackTotalTime
	)
	{
		//生成されたか
		bool needCreate = true;

		//生成時にカウントリセット
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
					//攻撃終了済のメモリを再利用
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
			//新規生成の必要がない
			return;
		}

		enemyAttackBoxes_.emplace_back(std::move(std::make_shared<T>(
			trans, size, attackData
		)));
	}

	/// <summary>
	/// 攻撃が終了しているか
	/// </summary>
	virtual const bool IsBoxesAttackEnd(void) const;

	virtual void BoxesClear(void);

private:

	//箱攻撃処理
	std::list<std::shared_ptr<EnemyAttack_Box>> enemyAttackBoxes_;

	//攻撃終了した箱攻撃のカウント
	int boxAttackCnt_;

	//箱攻撃の終了処理
	virtual void BoxAttackEnd(void);

};