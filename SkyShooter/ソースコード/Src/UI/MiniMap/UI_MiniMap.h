#pragma once

#include <memory>
#include "../Object/Common/Transform.h"
#include "../UIBase.h"

class ActorBase;
class CharactorBase;

/// <summary>
/// �~�j�}�b�v
/// </summary>
class UI_MiniMap :
    public UIBase
{
public:

    UI_MiniMap(void);
    virtual ~UI_MiniMap(void);

    /// <summary>
    /// �X�V����
    /// </summary>
    virtual void Update(void) override;

    /// <summary>
    /// �`�揈��
    /// </summary>
    virtual void Draw(void) override;

private:

    //�~�j�}�b�v�`��X�N���[��
    int miniMapScreen_;

    //���@�~�j�}�b�v���W
    Vector2f playerMiniMapPos_;

    //�G�~�j�}�b�v���W
    Vector2f ememyMiniMapPos_;

    //�~�j�}�b�v���W�̍X�V
    void UpdateMiniMapPos(void);

    //�~�j�}�b�v�O���b�h���̕`��
    void MiniMapGridDraw(void);

    //���@
    void MiniMapPlayerDraw(void);

    //�G    
    void MiniMapEnemyDraw(void);

};

