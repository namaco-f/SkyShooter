#pragma once
#include "../../ObjectTag.h"


/// <summary>
/// 追従するフレーム名
/// </summary>
enum class COLLISION_SUBTYPE
{
	NONE,
	MAX,
};

/// <summary>
/// 衝突判定の形状
/// </summary>
enum class COLLISION_TYPE
{
	NONE,		//初期化用
	SPHERE,		//球体
	CAPSULE,	//カプセル
	MODEL,		//モデル
	LINE,		//線分
	CYLINDER,	//シリンダー
};
