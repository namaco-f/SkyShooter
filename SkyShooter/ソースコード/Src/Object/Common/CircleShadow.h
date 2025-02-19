#pragma once
#include <vector>
#include "Transform.h"

/// <summary>
/// 丸影
/// </summary>
class CircleShadow
{
public:

	CircleShadow(void) = default;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="trans">追従するモデルの制御情報</param>
	/// <param name="polySize">影の大きさ</param>
	/// <param name="offSetY">モデルの座標からのオフセット値</param>
	CircleShadow(const Transform& trans,const VECTOR& polySize,const float offSetY = 0.0f);
	virtual ~CircleShadow(void);

	/// <summary>
	/// 初期化処理
	/// </summary>
	virtual void Init(void);

	/// <summary>
	/// 追従する座標を更新
	/// </summary>
	/// <param name="trans">座標情報</param>
	virtual void Update(const Transform& trans);

	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw(void);

private:

	//丸影のハンドルID
	int circleShadowHandle_;

	//丸影を反映する対象のワールド座標
	VECTOR transPos_;

	//回転
	Quaternion quaRot_;

	//ポリゴンのサイズ
	VECTOR polySize_;

	//ポリゴンのサイズの半分
	VECTOR polySizeHalf_;

	//影座標のY座標オフセット値
	float offSetY_;

	//ポリゴン配列
	std::vector<VERTEX3D> vertices_;

	//インデックス配列
	std::vector<unsigned short> index_;

	const VECTOR GetPolyPos(const VECTOR& size);

	//矩形ポリゴンを生成
	void MakeVertices(void);

	//矩形ポリゴンの更新
	void UpdateVertices(void);

	//y軸回転のみを取得
	const Quaternion GetQuaRotY(const Quaternion& qua);

	//回転反映後の頂点位置
	const VECTOR GetRotPos(const VECTOR size);

};

