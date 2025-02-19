#pragma once
#include <memory>
#include <string>
#include <future>
#include <functional>

class SceneManager;
class Camera;
class ResourceManager;
class ModelResourcesManager;
class SoundResourcesManager;
class EffectResourcesManager;
class ImageResourcesManager;
class ImagesResourcesManager;
class FontResourcesManager;
class ShaderManager;
class CharactorManager;
class ActorManager;
class UIManager;
class Collision;
class Observer;
class InputController;
class SoundController;
class Grid3d;

/// <summary>
/// �V�[�����N���X
/// </summary>
class SceneBase
{
public:

	SceneBase(void);
	virtual ~SceneBase(void);

	/// <summary>
	/// ������
	/// </summary>
	virtual void Init(void);

	/// <summary>
	/// �X�V����
	/// </summary>
	virtual void Update(void) = 0;

	/// <summary>
	/// ���ʍX�V
	/// </summary>
	virtual void UpdateSound(void);

	/// <summary>
	/// �`�揈��
	/// </summary>
	virtual void Draw(void) = 0;

	/// <summary>
	/// �񓯊����[�h
	/// </summary>
	virtual void ASyncLoad(void) = 0;

protected:

	//�Ǎ����
	enum class LoadState
	{
		None,			//�������p
		IsAsyncLoad,	//�񓯊��Ǎ���
		IsLoadEnd,		//�Ǎ��I��
	};

	//��ʏ��
	enum class State
	{
		None,			//�������p
		Load,			//�Ǎ���
		Play,			//���s��
		End,			//�I����
	};

	//���j���[�J�[�\���ړ����̃L�[�l
	const std::string SE_CURSORMOVE = "CursorMove";

	//���j���[���艹�̃L�[�l
	const std::string SE_ENTER = "Enter";

	//�V�[���Ǘ�
	SceneManager& scnMng_;

	//�J����
	Camera& camera_;

	// ���\�[�X�Ǘ�
	ResourceManager& resMng_;

	//���\�[�X�Ǘ� : ���f��
	ModelResourcesManager& resMngModel_;
	
	//���\�[�X�Ǘ� : ��
	SoundResourcesManager& resMngSound_;
	
	//���\�[�X�Ǘ� : �G�t�F�N�g
	EffectResourcesManager& resMngEffect_;
	
	//���\�[�X�Ǘ� : �摜
	ImageResourcesManager& resMngImage_;
	
	//���\�[�X�Ǘ� : �����摜
	ImagesResourcesManager& resMngImages_;
	
	//�V�F�[�_�[�Ǘ�
	ShaderManager& resMngShader_;
	
	//�t�H���g�Ǘ�
	FontResourcesManager& resMngFont_;

	//�I�u�W�F�N�g�Ǘ�
	ActorManager& actorMng_;

	//�L�����N�^�[�Ǘ�
	CharactorManager& charaMng_;

	//UI�Ǘ�
	UIManager& uiMng_;

	//�I�u�U�[�o�[
	Observer& observer_;

	//���͐���
	std::unique_ptr<InputController> inputController_;

	//������
	std::unique_ptr<SoundController> soundController_;

	//�O���b�h��
	std::unique_ptr<Grid3d> grid_;

	//�Ǎ����ԃJ�E���g
	float loadingTime_;


	//int* loadingImgsHandle_;
	//�Ǎ��摜�n���h��
	std::vector<int> loadingImgsHandles_;

	//�Ǎ��摜�C���f�b�N�X�J�E���g�p
	int loadingImgsIdxCnt_;

	//�Ǎ��摜�C���f�b�N�X���
	int loadingImgsIdxMax_;

	//�Ǎ����ԃJ�E���g
	float loadingCntStep_;

	//�Ǎ��I���摜�n���h��
	int loadingEndStartImgsHandle_;

	//��������摜
	std::vector<int> tutorialImgsHandle_;

	//����������ɕ\�����鍶�E���
	int tutorialArrowsImgsHandle_;

	//��������摜�C���f�b�N�X
	int tutorialImgsIdxCnt_;

	//��ʏ��
	State state_;

	//�Ǎ���Ԋm�F
	LoadState loadState_;

	//���̏�����
	virtual void InitSound(void);

	//��ʏ�ԑJ�ڂ̓o�^
	virtual void InitChangeState(void);

	//�Ǎ���ԑJ�ڂ̓o�^
	virtual void InitChangeLoadState(void);

	//��ʏ�ԑJ��
	virtual void ChangeState(const State state);

	//��ʏ�ԑJ�ڎ��̏�����
	std::unordered_map<State, std::function<void(void)>> stateChange_;
	virtual void ChangeState_Load(void);		//�Ǎ���
	virtual void ChangeState_Play(void);		//���s��
	virtual void ChangeState_End(void);			//�I����

	//��ʏ�Ԃ��Ƃ̍X�V
	std::function<void(void)> stateUpdate_;
	virtual void StateUpdate_Load(void);		//�Ǎ���
	virtual void StateUpdate_Play(void);		//���s��
	virtual void StateUpdate_End(void);			//�I����

	//��ʏ�Ԃ��Ƃ̕`��
	std::function<void(void)> stateDraw_;
	virtual void StateDraw_Load(void);			//�Ǎ���
	virtual void StateDraw_Play(void);			//���s��
	virtual void StateDraw_End(void);			//�I����

	//�Ǎ���ԑJ��
	virtual void ChangeLoadState(const LoadState loadState);

	//�Ǎ���ԑJ�ڎ��̏�����
	std::unordered_map<LoadState, std::function<void(void)>> loadStateChange_;
	virtual void ChangeLoadState_IsAsyncLoad(void);		//�񓯊��Ǎ���
	virtual void ChangeLoadState_IsLoadEnd(void);		//�Ǎ��I��

	//�Ǎ���Ԃ��Ƃ̍X�V
	std::function<void(void)> loadStateUpdate_;
	virtual void LoadStateUpdate_IsAsyncLoad(void);		//�񓯊��Ǎ���
	virtual void LoadStateUpdate_IsLoadEnd(void);		//�Ǎ��I��

	//�Ǎ���Ԃ��Ƃ̕`��
	std::function<void(void)> loadStateDraw_;
	virtual void LoadStateDraw_IsAsyncLoad(void);		//�񓯊��Ǎ���
	virtual void LoadStateDraw_IsLoadEnd(void);			//�Ǎ��I��

	virtual void UpdateTutorial(void);
	virtual void DrawTutorial(void);

	virtual void UpdateIsASyncLoad(void);
	virtual void DrawIsASyncLoad(void);

	//Dxlib��Ή��p�̔񓯊��Ǎ�
	std::future<void> aSyncLoad_;
	std::function<void(void)> effectLoad_;
};