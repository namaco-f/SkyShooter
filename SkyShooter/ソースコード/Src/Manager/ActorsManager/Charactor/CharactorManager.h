#pragma once

#include <memory>
#include <vector>
#include <functional>
#include <unordered_map>
#include "../Singleton.h"
#include "../Object/ObjectTag.h"
//#include "../Object/Actor/Charactor/CharactorBase.h"

class CharactorBase;

/// <summary>
/// キャラクター管理クラス
/// </summary>
class CharactorManager
	:public Singleton<CharactorManager>
{

public:

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(void);

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(void);

	/// <summary>
	/// キャラクター配列の取得
	/// </summary>
	/// <returns>キャラクター配列の参照</returns>
	inline const std::vector<std::shared_ptr<CharactorBase>>& GetCharactors(void) const
	{
		return charactors_;
	}

	/// <summary>
	/// 指定したキャラクターの数を取得
	/// </summary>
	/// <param name="objTag">指定するキャラクターのタグ</param>
	/// <returns>指定したキャラクターの数</returns>
	const int GetCharactorNum(const OBJECT_TYPE objTag) const;

	/// <summary>
	/// 指定したキャラクターを取得
	/// </summary>
	/// <returns>指定したキャラクターを取得</returns>
	template<class T>
	const std::shared_ptr<CharactorBase>& GetCharactor(const OBJECT_TYPE obj) const
	{
		//同一の複数オブジェクトに非対応
		for (const auto& charactor : charactors_)
		{
			if (charactor)
			{
				if (charactor->GetObjectType() == obj)
				{
					return charactor;
				}
			}
		}

		return nullCharactor_;
	}

	/// <summary>
	/// 指定したキャラクターを取得 : 複数用
	/// </summary>
	/// <returns>指定したキャラクターを取得 : 複数用</returns>
	template<class T>
	const std::list<std::weak_ptr<CharactorBase>> GetCharactors(const OBJECT_TYPE obj)
	{
		std::list<std::weak_ptr<CharactorBase>> ret;
		
		//同一の複数オブジェクトに対応
		for (const auto& charactor : charactors_)
		{
			if (charactor)
			{
				if (charactor->GetObjectType() == obj)
				{
					ret.emplace_back(charactor);
				}
			}
		}
		return ret;
	}

	/// <summary>
	/// キャラクターの追加
	/// </summary>
	/// <param name="createFunc">
	/// 対象のキャラクターのCreate関数
	/// </param>
	void AddCharactor(std::function<std::shared_ptr<CharactorBase>()> createFunc);

	/// <summary>
	/// キャラクターの削除
	/// </summary>
	inline void ClearCharactors(void)
	{
		if (charactorsNums_.empty() && charactors_.empty())
		{
			return;
		}

		charactors_.clear();
		charactorsNums_.clear();
	}

private:

	friend class Singleton<CharactorManager>;

	//キャラクター : 結局全部回す為、listにすべき
	std::vector<std::shared_ptr<CharactorBase>> charactors_;

	//キャラクター数カウント用配列
	std::unordered_map<OBJECT_TYPE,int> charactorsNums_;

	//空キャラクター
	const std::shared_ptr<CharactorBase> nullCharactor_ = nullptr;

	CharactorManager(void);
	virtual ~CharactorManager(void) override;
};

