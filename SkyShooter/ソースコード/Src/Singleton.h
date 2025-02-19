#pragma once

/// <summary>
/// シングルトンテンプレート
/// </summary>
template<class T>
class Singleton
{

public:

	/// <summary>
	/// 静的なインスタンスを明示的に生成
	/// </summary>
	static inline void CreateInstace(void)
	{
		//既に存在しているなら終了
		if (instance_) { return; }

		instance_ = new T();
		instance_->Init();
	}

	/// <summary>
	/// 静的なインスタンスの取得
	/// </summary>
	/// <returns></returns>
	static inline T& GetInstance()
	{
		return *instance_;
	}

	/// <summary>
	/// リソース解放 : 明示的にインスタンスの削除
	/// </summary>
	/// <remarks>Dxlib_End()前に解放する必要がある</remarks>
	virtual void Destroy(void) 
	{
		//インスタンスの削除
		delete instance_;
	};

	/// <summary>
	/// コピーの禁止
	/// </summary>
	Singleton(const Singleton& singleton) = delete;
	Singleton& operator=(const Singleton& singleton) = delete;
	Singleton(Singleton&& singleton) = delete;
	Singleton& operator=(Singleton&& singleton) = delete;

protected:

	/// <summary>
	/// 静的なインスタンス
	/// </summary>
	static T* instance_;

	Singleton() = default;
	virtual ~Singleton() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <remarks>実体を生成後に初期化が必要なもの用</remarks>
	virtual void Init(void) {};

};

