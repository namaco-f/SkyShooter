#pragma once

#include <functional>
#include <memory>

class PlayerBase;
class InputController;

/// <summary>
/// プレイヤーの入力検知
/// </summary>
class PlayerInputComponent
{

public:

    PlayerInputComponent(void);
    virtual ~PlayerInputComponent(void);

    /// <summary>
    /// 更新処理
    /// </summary>
    /// <param name="player">入力を通知する自機の参照</param>
    virtual void Update(PlayerBase& player);

private:

    //入力制御
    std::unique_ptr<InputController> inputController_;

    //デルタタイム
    float deltaTime_;

    //移動関連
    void InputUpdateMove(PlayerBase& player);

    //攻撃関連
    void InputUpdateAttack(PlayerBase& player);     

    //他にキーが押されていない場合
    const bool IsNotMoving(void);
};

