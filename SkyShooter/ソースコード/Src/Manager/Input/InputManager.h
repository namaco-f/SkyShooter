#pragma once
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <Dxlib.h>
#include "InputControllerTag.h"
#include "../../Singleton.h"

class InputUpdater;

/// <summary>
/// ���͐���̓o�^
/// </summary>
class InputManager
	: public Singleton<InputManager>
{
public:

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update(void);

	/// <summary>
	/// �W���C�p�b�h�̎�ނ��擾
	/// </summary>
	JOYPAD_TYPE GetJPadType(JOYPAD_NO no);

	/// <summary>
	/// �}�E�X���W���擾
	/// </summary>
	/// <param name="pos">�}�E�X���W����������Vector2</param>
	void GetMousePos(Vector2& pos);

	/// <summary>
	/// ����`�����w��
	/// </summary>
	void SetControlType(CONTROL_TYPE type);

	/// <summary>
	/// ����`�����擾
	/// </summary>
	CONTROL_TYPE GetControlType(void);

	/// <summary>
	/// ���݂̓��͏��z����擾
	/// </summary>
	/// <returns>currentInput_</returns>
	const std::unordered_map<ACTION, bool>& GetCurrentInput(void);

	/// <summary>
	/// �w�肵���A�N�V�����̓��͏����擾
	/// </summary>
	/// <param name="act">�A�N�V����</param>
	/// <returns>���͔���</returns>
	const bool GetCurrentInput(ACTION act);
	
	/// <summary>
	/// 1f�O�̓��͏��z����擾
	/// </summary>
	/// <returns>lastInput_</returns>
	const std::unordered_map<ACTION, bool>& GetLastInput(void);

	/// <summary>
	/// �w�肵���A�N�V������1f�O�̓��͏����擾
	/// </summary>
	/// <param name="act">�A�N�V����</param>
	/// <returns>���͔���</returns>
	const bool GetLastInput(ACTION act);

private:

	friend class Singleton<InputManager>;

	//���͂̍X�V
	std::unique_ptr<InputUpdater> inputUpdater_;

	//���͂ƃA�N�V�����̑Ή��e�[�u��
	std::unordered_map<ACTION, std::vector<InputMapInfo>> inputActionMaps_;

	//���݂̓��͏��
	std::unordered_map<ACTION, bool> currentInput_;

	//���͂̎��
	INPUT_TYPE currentInputType_;

	//���O�̓��͏��
	std::unordered_map<ACTION, bool> lastInput_;

	//����`��
	CONTROL_TYPE controlType_;

	InputManager(void);
	virtual ~InputManager(void) override;
	
	//�ڑ�����Ă���R���g���[���[�̐���Ԃ�
	int GetJPadNum(void);

};
