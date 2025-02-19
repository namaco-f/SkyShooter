#pragma once

#include <vector>
#include <memory>
#include <unordered_map>
#include <functional>
#include "../Singleton.h"
#include "../Object/ObjectTag.h"

class ActorBase;

/// <summary>
/// Charactorや弾以外のActorを管理
/// </summary>
class ActorManager
	: public Singleton<ActorManager>
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
	inline const std::list<std::shared_ptr<ActorBase>>& GetActors(void) const
	{
		return actors_;
	}

	/// <summary>
	/// 指定したキャラクターを取得
	/// </summary>
	/// <returns>指定したキャラクターを取得</returns>
	template<class T>
	const std::shared_ptr<ActorBase>& GetActor(const OBJECT_TYPE obj) const
	{
		//同一の複数オブジェクトに非対応
		for (const auto& actor : actors_)
		{
			if (actor)
			{
				if (actor->GetObjectType() == obj)
				{
					return actor;
				}
			}
		}

		return nullActor_;
	}

	/// <summary>
	/// 指定したキャラクターを取得 : 複数用
	/// </summary>
	/// <returns>指定したキャラクターを取得 : 複数用</returns>
	template<class T>
	const std::list<std::weak_ptr<ActorBase>> GetActors(const OBJECT_TYPE obj)
	{
		std::list<std::weak_ptr<ActorBase>> ret;

		//同一の複数オブジェクトに対応
		for (const auto& actor : actors_)
		{
			if (actor)
			{
				if (actor->GetObjectType() == obj)
				{
					ret.emplace_back(actor);
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
	void AddActor(std::function<std::shared_ptr<ActorBase>()> createFunc);

	inline void ClearActors(void)
	{
		if (actorsNums_.empty() && actors_.empty())
		{
			return;
		}

		actors_.clear();
		actorsNums_.clear();
	}

private:

	friend class Singleton<ActorManager>;

	//アクター数
	std::list<std::shared_ptr<ActorBase>> actors_;

	//アクター数カウント用配列
	std::unordered_map<OBJECT_TYPE, int> actorsNums_;

	//空アクター
	const std::shared_ptr<ActorBase> nullActor_ = nullptr;

	ActorManager(void);
	virtual ~ActorManager(void) override;

};

