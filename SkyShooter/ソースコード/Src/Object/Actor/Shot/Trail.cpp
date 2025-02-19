
#include "../Application.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "../Manager/ResourceManager/ResourceManager.h"
#include "../Manager/ResourceManager/Image/ImageResourcesManager.h"
#include "../Manager/ResourceManager/Shader/ShaderManager.h"
#include "../ActorBase.h"
#include "../Utility/MathUtility.h"
#include "../Renderer/Renderer.h"
#include "../Renderer/Material/PixelMaterial.h"
#include "../Renderer/Material/VertexsMaterial.h"
#include "Trail.h"

using namespace std;

using Math = MathUtility;

const float TRAIL_LENGTH = 50.0f;		//�g���C������̒���
const int TRAIL_LIST_MAX = 20;			//�g���C���z��̍ő吔
const float TRAIL_WIDTH = 20.0f;		//�g���C���̕�

const float FLOATU8_COLOR_MAX = 255.0f;
const float ALPHA = 0.8f;

const COLOR_U8 DEF_COLOR = { 255,255,255,255 };
const VECTOR NORM = { 0.0f,1.0f,0.0f };

Trail::Trail(const Transform& trans,const VECTOR& color)
	:
	shaderMng_(ResourceManager::GetInstance().GetShaderResourcesInstance()),
	transformPos_(trans.pos),
	trailNumMax_(TRAIL_LIST_MAX),
	trailLength_(TRAIL_LENGTH)
{
	//screen_ = MakeScreen(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, true);
	AddTrailPosList(transformPos_);
	//InitRender();
	
	GetColorVECTOR(color);

	auto& image = ResourceManager::GetInstance().GetImageResourcesInstance();
	tex_ = image.Load(SRC_IMAGE::TrailTexture).GetHandleId();
}

Trail::~Trail(void)
{
}

void Trail::Update(const Transform& trans)
{
	transformPos_ = trans.pos;

	CalculateTrail();
}

void Trail::Draw(void)
{
	if (trailPosList_.empty() || trailPosList_.size() <= 1)
	{
		//�g���C���̔z�񂪑��݂��Ȃ�����������Ă��Ȃ�
		return;
	}

	//list�^�Ȃ��߁A�C�e���[�^���g���Ďw��
	auto it = trailPosList_.begin();
	list<VECTOR>::iterator it2 = it;
	++it2;

	//�|���S���z��
	std::vector<VERTEX3D> vertices(trailPosList_.size() * 2);
	size_t idx = 0;

	//�O�Ղ̐��x�N�g����z��Ƃ��Ċi�[
	vector<VECTOR> lineVec;
	lineVec.reserve(trailPosList_.size() - 1);


	while (it2 != trailPosList_.end())
	{
		//���W�z�񂪏I�[�ł͂Ȃ�

		lineVec.emplace_back(VSub(*it2, *it));
		++it;
		++it2;
	}

	//�J�������W
	VECTOR cameraPos = GetCameraPosition();

	it = trailPosList_.begin();
	auto linVecIt = lineVec.begin();
	
	//�O�Ղ̐��x�N�g���̃|���S�������p�x�N�g��
	VECTOR vec = {};

	while (it != trailPosList_.end())
	{
		//���W�z�񂪏I�[�ł͂Ȃ���

		if (linVecIt != lineVec.end())
		{
			//�O�Ղ̐��x�N�g���z�񂪏I�[�ł͂Ȃ�

			//�����x�N�g��
			VECTOR lookAtVec = VNorm(VSub(*it,cameraPos));

			//�O�Ղ̐��x�N�g���ɑ΂��Đ����ȃx�N�g��
			VECTOR binormal = VNorm(VCross(*linVecIt, lookAtVec));

			if (linVecIt + 1 != lineVec.end())
			{
				//���̍��W�����݂��Ă���
				//�O�Ղ̐��x�N�g���ɑ΂��Đ����ȃx�N�g��
				vec = VNorm(VCross(*(linVecIt + 1), lookAtVec));
			}
			else
			{
				//���̍��W�����݂��Ă��Ȃ�
				vec = binormal;
			}

			//��[�̃|���S���𐶐�
			vertices[idx].pos = VAdd(*it, VScale(vec, TRAIL_WIDTH));
			vertices[idx].dif = diffuseColor_;
			vertices[idx].spc = DEF_COLOR;
			vertices[idx].norm = NORM;
			vertices[idx].su = 0.0f;
			vertices[idx].sv = 0.0f;
			vertices[idx].u = (float)idx / (float)(vertices.size());
			vertices[idx].v = 1.0f;

			//���[�̃|���S���𐶐�
			vertices[idx + 1].pos = VSub(*it, VScale(vec, TRAIL_WIDTH));
			vertices[idx + 1].dif = diffuseColor_;
			vertices[idx + 1].spc = DEF_COLOR;
			vertices[idx + 1].norm = NORM;
			vertices[idx + 1].su = 0.0f;
			vertices[idx + 1].sv = 0.0f;
			vertices[idx + 1].u = (float)idx / (float)(vertices.size());
			vertices[idx + 1].v = 0.0f;

			//�|���S���̃C���f�b�N�X�����̃|���S����
			idx += 2;
		}

		//���W�z��̃C�e���[�^������
		++it;
	}

	//�o�b�N�J�����O������
	SetUseBackCulling(false);
	
	DrawPrimitive3D(vertices.data(), (int)vertices.size(), DX_PRIMTYPE_TRIANGLESTRIP, tex_, true);

	//�o�b�N�J�����O�L����
	SetUseBackCulling(true);
}

void Trail::AddTrailPosList(const VECTOR& addTrailPos)
{
	trailPosList_.emplace_back(addTrailPos);

	int size = (int)trailPosList_.size();


	if (size < TRAIL_LIST_MAX)
	{
		//�g���C���̍��W���K��̐��ȉ��Ȃ̂ŏI��
		return;
	}

	//�Ō�̍��W���|�b�v
	trailPosList_.pop_front();
}

void Trail::GetColorVECTOR(const VECTOR& color)
{
	diffuseColor_ = GetColorU8(
		(int)(FLOATU8_COLOR_MAX * color.x),
		(int)(FLOATU8_COLOR_MAX * color.y),
		(int)(FLOATU8_COLOR_MAX * color.z),
		(int)(FLOATU8_COLOR_MAX * ALPHA)
	);
}

void Trail::CalculateTrail(void)
{
	if (trailPosList_.empty())
	{
		//�g���C���p���W�̔z�񂪑��݂��Ȃ�
		return;
	}

	//�ŐV�̃g���C���p���W�̎擾
	const VECTOR& trailEndPos = trailPosList_.back();
	const VECTOR& pos = transformPos_;

	//���݂̍��W�ƃg���C���p���W�̒������v�Z
	float length = Math::MagnitudeF(VSub(pos, trailEndPos));

	if (length > TRAIL_LENGTH)
	{
		//�g���C���p���W�ɒǉ�
		AddTrailPosList(transformPos_);
	}
}
