#pragma once
#include "../Type/StandardShot.h"

/// <summary>
/// ���@�W���e
/// </summary>
class PlayerShot_Standard 
    : public StandardShot
{

public:

    PlayerShot_Standard(void) = default;
    virtual ~PlayerShot_Standard(void);

    /// <summary>
    /// �R���X�g���N�^
    /// </summary>
    /// <param name="trans">�e���ˎ��̍��W</param>
    explicit PlayerShot_Standard(const Transform& trans);

    /// <summary>
    /// �`�揈��
    /// </summary>
    virtual void Draw(void) override;

    /// <summary>
    /// �e���˂̍��W�ݒ�
    /// </summary>
    void InitShotStartPos(void) override;

    ///// <summary>
    ///// �e�̐�������
    ///// </summary>
    ///// <returns>
    ///// true : ����
    ///// false : �j��
    ///// </returns>
    //virtual const bool IsAlive(void) const override;

private:
  
    //���������̓Ǎ�
    void InitLoad(void) override;
    
    //���f��������̏�����
    void InitTransform(void) override;

    //�Փ˔���̏�����
    void InitCollider(void) override;

    //�S�Ă̏������I����̏���
    void InitEnd(void) override;
 
    //�G�ɏՓ˂��Ă���
    void HitEnemy(void) override;

    virtual void DrawDebugModel(void);

    virtual void AddChangeState(void) override;
    virtual void Change_None(void);
    virtual void Change_Blast(void);
    virtual void Change_End(void);

    virtual void Update_Blast(void) override;

    virtual void Draw_Blast(void) override;

};

