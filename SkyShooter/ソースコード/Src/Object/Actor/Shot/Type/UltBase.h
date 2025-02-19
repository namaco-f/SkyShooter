#pragma once
#include <unordered_Map>
#include <list>
#include "../ShotBase.h"


class UltBase :
    public ShotBase
{
public:

    /// <summary>
    /// ó‘Ô
    /// </summary>
    enum class State
    {
        None,       //‰Šú‰»—p
        Charge,     //UŒ‚‘O
        Attack,     //UŒ‚’†
        Blast,      //”š”­
        End,        //I—¹
        MAX,        //Å‘å”ƒJƒEƒ“ƒg—p
    };

    UltBase(void) = default;
    virtual ~UltBase(void) = default;

    explicit UltBase(const Transform& transform);

    virtual void Draw(void) override;

    /// <summary>
    /// ó‘Ô‘JˆÚ
    /// </summary>
    /// <param name="state">ó‘Ô</param>
    virtual void ChangeState(const State state);

protected:


    //ó‘Ô
    State state_;

    //ó‘Ô•Ï‰»‚ÌŠÖ”‚Ì“o˜^
    virtual void AddStateChange(void) = 0;

    //’e”­Ë‚Ìİ’è
    virtual void InitShotStartPos(void) = 0;

    //’e‚Ì”­Ë•ûŒü‚ğİ’è
    virtual void InitShotVec(void);

    //ó‘Ô‚²‚Æ‚ÌXV
    void UpdateState(void) override;

    //ˆÚ“®ˆ—
    void Move(void) override;

    //ó‘Ô•Ï‰»‚És‚í‚ê‚éŠÖ”
    std::unordered_map<State, std::function<void(void)>> stateChange_;

    //ó‘Ô‚²‚Æ‚ÌXVˆ—
    std::function<void(void)> stateUpdate_;

    //’e‚Ì”j‰óˆ—
    void Destroy(void) override;

    //”j‰ó‚³‚ê‚Ä‚¢‚é‚©
    virtual const bool IsDestroy(void) const override;
};

