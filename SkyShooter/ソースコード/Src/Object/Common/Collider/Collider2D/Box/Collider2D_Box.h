#pragma once
#include "../Collider2D.h"


/// <summary>
/// 矩形の2Dコライダの基底クラス
/// </summary>
class Collider2D_Box :
    public Collider2D
{
public:

    /// <summary>
    /// 矩形の座標情報
    /// </summary>
    struct BoxData
    {
        Vector2f posLeftTop;
        Vector2f posLeftBottom;

        Vector2f posRightTop;
        Vector2f posRightBottom;
    };

    Collider2D_Box(void) = default;

	/// <summary>
    /// コンストラクタ
    /// </summary>
    /// <param name="colData">2Dコライダ情報</param>
	explicit Collider2D_Box(const ColliderData_2D& colData, const bool colliderDef = true);
    virtual ~Collider2D_Box(void);

    /// <summary>
    /// コライダの描画
    /// </summary>
    virtual void Draw(void)override;

    /// <summary>
    /// 制御情報の更新
    /// </summary>
    /// <param name="trans">制御情報</param>
    virtual void UpdateTransform(const Transform& trans) override;

protected:

    //矩形の座標情報
    BoxData boxData_;

    //矩形の座標を計算
    virtual void CalculateBoxPos(void) = 0;

    //Vector2fをVECTORに変換
    virtual const VECTOR Vector2fToVECTOR(const Vector2f& pos) = 0;

    //コライダの色を取得
    virtual const unsigned int GetColliderColor(void)const;

    virtual const VECTOR GetDirection(void) const = 0;

private:
};

