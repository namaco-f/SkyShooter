#pragma once
#include <Dxlib.h>

/// <summary>
/// �f�o�b�O�p�O���b�h���̕`��
/// </summary>
class Grid3d
{
public:

	Grid3d(void);
	virtual ~Grid3d(void);

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update(void);

	/// <summary>
	/// �`�揈��
	/// </summary>
	void Draw(void);
};

