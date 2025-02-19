#pragma once

#include <functional>
#include <unordered_map>

/// <summary>
/// �t�F�[�h�C��,�t�F�[�h�A�E�g���s���N���X
/// </summary>
class Fader
{
public:

	/// <summary>
	/// �t�F�[�h�̏��
	/// </summary>
	enum class State
	{
		NONE, 
		FADE_OUT,	//�t�F�[�h�A�E�g
		FADE_IN		//�t�F�[�h�C��
	};

	Fader(void);
	virtual ~Fader(void);

	/// <summary>
	/// �t�F�[�h��Ԃ̎擾
	/// </summary>
	/// <returns>state_</returns>
	State GetState(void) const;

	/// <summary>
	/// �t�F�[�h�������I�����Ă��邩
	/// </summary>
	bool IsEnd(void) const;

	/// <summary>
	/// �w�肵���t�F�[�h���J�n
	/// </summary>
	void ChangeState(const State state);

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update(void);

	/// <summary>
	/// �`�揈��
	/// </summary>
	void Draw(void);

private:

	//���
	State state_;

	//�����x
	int alpha_;

	//���(State)��ۂ����܂܏I��������s�����߁A
	//Update->Draw->Update��1�t���[������p
	bool isPreEnd_;

	//�t�F�[�h�����̏I������
	bool isEnd_;

	//��ԑJ��
	std::unordered_map<State, std::function<void(void)>> changeStates_;
	void ChangeState_None(void);		//�t�F�[�h�Ȃ�
	void ChangeState_FadeOut(void);		//�t�F�[�h�A�E�g
	void ChangeState_FadeIn(void);		//�t�F�[�h�C��

	//��Ԃ��Ƃ̍X�V����
	std::function<void(void)> stateUpdate_;
	void StateUpdate_None(void);		//�t�F�[�h�Ȃ�
	void StateUpdate_FadeOut(void);		//�t�F�[�h�A�E�g
	void StateUpdate_FadeIn(void);		//�t�F�[�h�C��

	//��Ԃ��Ƃ̕`�揈��
	std::function<void(void)> stateDraw_;
	void StateDraw_None(void);			//�t�F�[�h�Ȃ�
	void StateDraw_FadeOut(void);		//�t�F�[�h�A�E�g
	void StateDraw_FadeIn(void);		//�t�F�[�h�C��

	//��ԑJ�ڂ̓o�^
	void InitChangeState(void);
};
