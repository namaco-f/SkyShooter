#pragma once
#include <Dxlib.h>

/// <summary>
/// デバッグ用グリッド線の描画
/// </summary>
class Grid3d
{
public:

	Grid3d(void);
	virtual ~Grid3d(void);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(void);

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(void);
};

