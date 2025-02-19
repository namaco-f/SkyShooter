#pragma once
//#include <memory>
#include <Dxlib.h>

/// <summary>
/// ライトの操作
/// </summary>
class LigController
{
public:

	/// <summary>
	/// ライト情報
	/// </summary>
	struct LightInfo
	{
		VECTOR pos;		//ライト座標
		VECTOR dir;		//ライト方向
		VECTOR col;		//ライトの色

		LightInfo(void)
			:
			pos{},
			dir{},
			col{}
		{};
	};

	LigController(void);
	~LigController(void);

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init(void);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(void);

	/// <summary>
	/// ライトカラーの設定
	/// </summary>
	void SetLigColor(const VECTOR& col);

	/// <summary>
	/// ライトカラーの取得
	/// </summary>
	const VECTOR& GetLigColor(void) const;
	
	/// <summary>
	/// ライト方向の設定
	/// </summary>
	void SetLigDir(const VECTOR& dir);

	/// <summary>
	/// ライト方向の取得
	/// </summary>
	const VECTOR GetLigDir(void) const;
	
	/// <summary>
	/// ライト座標の設定
	/// </summary>
	void SetLigPos(const VECTOR& pos);

	/// <summary>
	/// ライト座標の取得
	/// </summary>
	const VECTOR& GetLigPos(void);


private:

	//ライト情報
	LightInfo ligInfo_;

};

