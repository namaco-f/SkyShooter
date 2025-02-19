#pragma once
//#include <memory>
#include <Dxlib.h>

/// <summary>
/// ���C�g�̑���
/// </summary>
class LigController
{
public:

	/// <summary>
	/// ���C�g���
	/// </summary>
	struct LightInfo
	{
		VECTOR pos;		//���C�g���W
		VECTOR dir;		//���C�g����
		VECTOR col;		//���C�g�̐F

		LightInfo(void)
			:
			pos{},
			dir{},
			col{}
		{};
	};

	LigController(void);
	~LigController(void);

	/// <summary>
	/// ����������
	/// </summary>
	void Init(void);

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update(void);

	/// <summary>
	/// ���C�g�J���[�̐ݒ�
	/// </summary>
	void SetLigColor(const VECTOR& col);

	/// <summary>
	/// ���C�g�J���[�̎擾
	/// </summary>
	const VECTOR& GetLigColor(void) const;
	
	/// <summary>
	/// ���C�g�����̐ݒ�
	/// </summary>
	void SetLigDir(const VECTOR& dir);

	/// <summary>
	/// ���C�g�����̎擾
	/// </summary>
	const VECTOR GetLigDir(void) const;
	
	/// <summary>
	/// ���C�g���W�̐ݒ�
	/// </summary>
	void SetLigPos(const VECTOR& pos);

	/// <summary>
	/// ���C�g���W�̎擾
	/// </summary>
	const VECTOR& GetLigPos(void);


private:

	//���C�g���
	LightInfo ligInfo_;

};

