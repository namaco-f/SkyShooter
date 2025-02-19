#pragma once
#include <string>
#include "Singleton.h"

/// <summary>
/// �N�����̏������A�I�����̉���A�Q�[�����[�v���s���N���X
/// </summary>
class Application :
	public Singleton<Application>
{

public:

	static constexpr int SCREEN_SIZE_X = 1600;						//�X�N���[���T�C�YX
	static constexpr int SCREEN_SIZE_Y = 900;						//�X�N���[���T�C�YY
	static constexpr int SCREEN_SIZE_HALF_X = SCREEN_SIZE_X / 2;	//�X�N���[���T�C�YX�̔���
	static constexpr int SCREEN_SIZE_HALF_Y = SCREEN_SIZE_Y / 2;	//�X�N���[���T�C�YY�̔���

	static constexpr int DEBUG_STR_DIFF_Y_INT = 20;					//������̊Ԋu : �f�o�b�O�p
	static constexpr float DEBUG_STR_DIFF_Y_F = 20.0f;				//������̊Ԋu : �f�o�b�O�p

	static constexpr int HANDLE_NULL = -1;							//�n���h��ID�G���[

	//�f�[�^�p�X�֘A
	static const std::string PATH_IMAGE;			//�摜
	static const std::string PATH_MODEL;			//���f��
	static const std::string PATH_EFFECT;			//�G�t�F�N�g
	static const std::string PATH_SOUND;			//��
	static const std::string PATH_FONT;				//�t�H���g
	static const std::string PATH_ANIMATION;		//�A�j���[�V����
	static const std::string PATH_SHADER;			//�V�F�[�_�[
	static const std::string PATH_SHADER_PIXEL;		//�s�N�Z���V�F�[�_�[
	static const std::string PATH_SHADER_VERTEXS;	//���_�V�F�[�_�[
	static const std::string PATH_JSON;				//JSON
	static const std::string PATH_BINARY;			//�o�C�i��

	/// <summary>
	/// �Q�[�����[�v�̊J�n
	/// </summary>
	void Run(void);

	/// <summary>
	/// ���\�[�X�̉��
	/// </summary>
	/// <remarks>DxlibEnd()�O�ɉ����������������</remarks>
	virtual void Destroy(void) override;

	/// <summary>
	/// Dxlib�̏����������^���s�̔���
	/// </summary>
	/// <returns>
	/// ������ : true
	/// ���s�� : false
	/// </returns>
	const bool IsInitFail(void) const;

private:

	friend class Singleton<Application>;

	//���������s
	bool isInitFail_;

	//������s
	bool isReleaseFail_;

	//�f�t�H���g�R���X�g���N�^
	Application(void);
	virtual ~Application(void);

	//Effekseer�̏�����
	void InitEffekseer(void);

	void CalcFrameRate(void);

	void DrawFrameRateCnt(void);

	int currentTime_ = 0;
	int lastTime_ = 0;

	int frameCnt_ = 0;
	int updateFrameRateTime_ = 0;

	float frameRate_ = 0.0f;

		
};

