#pragma once

/// <summary>
/// �V���O���g���e���v���[�g
/// </summary>
template<class T>
class Singleton
{

public:

	/// <summary>
	/// �ÓI�ȃC���X�^���X�𖾎��I�ɐ���
	/// </summary>
	static inline void CreateInstace(void)
	{
		//���ɑ��݂��Ă���Ȃ�I��
		if (instance_) { return; }

		instance_ = new T();
		instance_->Init();
	}

	/// <summary>
	/// �ÓI�ȃC���X�^���X�̎擾
	/// </summary>
	/// <returns></returns>
	static inline T& GetInstance()
	{
		return *instance_;
	}

	/// <summary>
	/// ���\�[�X��� : �����I�ɃC���X�^���X�̍폜
	/// </summary>
	/// <remarks>Dxlib_End()�O�ɉ������K�v������</remarks>
	virtual void Destroy(void) 
	{
		//�C���X�^���X�̍폜
		delete instance_;
	};

	/// <summary>
	/// �R�s�[�̋֎~
	/// </summary>
	Singleton(const Singleton& singleton) = delete;
	Singleton& operator=(const Singleton& singleton) = delete;
	Singleton(Singleton&& singleton) = delete;
	Singleton& operator=(Singleton&& singleton) = delete;

protected:

	/// <summary>
	/// �ÓI�ȃC���X�^���X
	/// </summary>
	static T* instance_;

	Singleton() = default;
	virtual ~Singleton() = default;

	/// <summary>
	/// ������
	/// </summary>
	/// <remarks>���̂𐶐���ɏ��������K�v�Ȃ��̗p</remarks>
	virtual void Init(void) {};

};

