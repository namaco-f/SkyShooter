#pragma once

#include <memory>
#include "../Object/Common/Transform.h"
#include "../UIBase.h"

class ActorBase;
class CharactorBase;

/// <summary>
/// ミニマップ
/// </summary>
class UI_MiniMap :
    public UIBase
{
public:

    UI_MiniMap(void);
    virtual ~UI_MiniMap(void);

    /// <summary>
    /// 更新処理
    /// </summary>
    virtual void Update(void) override;

    /// <summary>
    /// 描画処理
    /// </summary>
    virtual void Draw(void) override;

private:

    //ミニマップ描画スクリーン
    int miniMapScreen_;

    //自機ミニマップ座標
    Vector2f playerMiniMapPos_;

    //敵ミニマップ座標
    Vector2f ememyMiniMapPos_;

    //ミニマップ座標の更新
    void UpdateMiniMapPos(void);

    //ミニマップグリッド線の描画
    void MiniMapGridDraw(void);

    //自機
    void MiniMapPlayerDraw(void);

    //敵    
    void MiniMapEnemyDraw(void);

};

