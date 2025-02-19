#pragma once

#include <unordered_map>
#include "InputControllerTag.h"
#include <DxLib.h>

class InputManager;

/// <summary>
/// ���͌��m�N���X
/// </summary>
class InputController
{
public:

	InputController(void);
	virtual ~InputController(void);

	/// <summary>
	/// ���̓`�F�b�N
	/// </summary>
	/// <param name="action">�A�N�V�����̖��O</param>
	/// <returns>
	/// true : ������Ă���
	/// false : ������ĂȂ�
	/// </returns>
	bool IsPressed(const ACTION& action) const;

	/// <summary>
	/// ���̓`�F�b�N
	/// </summary>
	/// <param name="action">���ׂ����A�N�V����</param>
	/// <returns>
	/// true : �������ꂽ
	/// false : ������ĂȂ����������ςȂ�
	/// </returns>
	bool IsTriggered(const ACTION& action) const;

	/// <summary>
	/// ���̓`�F�b�N
	/// </summary>
	/// <param name="action">���ׂ����A�N�V����</param>
	/// <returns>
	/// true:�������ꂽ
	/// </returns>
	bool IsReleased(const ACTION& action) const;

private:

	//���͂̓o�^���
	InputManager& inputMng_;
};