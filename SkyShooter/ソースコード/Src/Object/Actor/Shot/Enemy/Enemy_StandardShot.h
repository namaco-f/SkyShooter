#pragma once
#include "../Type/StandardShot.h"

class Enemy_StandardShot :
    public StandardShot
{

public:
    
    Enemy_StandardShot(void) = default;
    explicit Enemy_StandardShot(const Transform& trans);
    virtual ~Enemy_StandardShot(void);

private:

    /// <summary>
    /// �`�揈��
    /// </summary>
    virtual void Draw(void) override;

    //�e���˂̐ݒ�
    void InitShotStartPos(void) override;

    /// <summary>
    /// �e�̐�������
    /// </summary>
    /// <returns>
    /// true : ����
    /// false : �j��
    /// </returns>
    virtual const bool IsAlive(void) const override;

private:

    //���\�[�X�̓Ǎ�
    void InitLoad(void) override;

    //���f��������̍X�V
    void InitTransform(void) override;

    void InitCollider(void) override;
    void InitEnd(void) override;

    void HitPlayer(void) override;

    virtual void AddChangeState(void) override;
    virtual void Change_None(void);
    virtual void Change_Blast(void);
    virtual void Change_End(void);

};

