
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

const float TRAIL_LENGTH = 50.0f;		//トレイル一つ分の長さ
const int TRAIL_LIST_MAX = 20;			//トレイル配列の最大数
const float TRAIL_WIDTH = 20.0f;		//トレイルの幅

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
		//トレイルの配列が存在しないか数が足りていない
		return;
	}

	//list型なため、イテレータを使って指定
	auto it = trailPosList_.begin();
	list<VECTOR>::iterator it2 = it;
	++it2;

	//ポリゴン配列
	std::vector<VERTEX3D> vertices(trailPosList_.size() * 2);
	size_t idx = 0;

	//軌跡の線ベクトルを配列として格納
	vector<VECTOR> lineVec;
	lineVec.reserve(trailPosList_.size() - 1);


	while (it2 != trailPosList_.end())
	{
		//座標配列が終端ではない

		lineVec.emplace_back(VSub(*it2, *it));
		++it;
		++it2;
	}

	//カメラ座標
	VECTOR cameraPos = GetCameraPosition();

	it = trailPosList_.begin();
	auto linVecIt = lineVec.begin();
	
	//軌跡の線ベクトルのポリゴン生成用ベクトル
	VECTOR vec = {};

	while (it != trailPosList_.end())
	{
		//座標配列が終端ではない間

		if (linVecIt != lineVec.end())
		{
			//軌跡の線ベクトル配列が終端ではない

			//視線ベクトル
			VECTOR lookAtVec = VNorm(VSub(*it,cameraPos));

			//軌跡の線ベクトルに対して垂直なベクトル
			VECTOR binormal = VNorm(VCross(*linVecIt, lookAtVec));

			if (linVecIt + 1 != lineVec.end())
			{
				//次の座標が存在している
				//軌跡の線ベクトルに対して垂直なベクトル
				vec = VNorm(VCross(*(linVecIt + 1), lookAtVec));
			}
			else
			{
				//次の座標が存在していない
				vec = binormal;
			}

			//上端のポリゴンを生成
			vertices[idx].pos = VAdd(*it, VScale(vec, TRAIL_WIDTH));
			vertices[idx].dif = diffuseColor_;
			vertices[idx].spc = DEF_COLOR;
			vertices[idx].norm = NORM;
			vertices[idx].su = 0.0f;
			vertices[idx].sv = 0.0f;
			vertices[idx].u = (float)idx / (float)(vertices.size());
			vertices[idx].v = 1.0f;

			//下端のポリゴンを生成
			vertices[idx + 1].pos = VSub(*it, VScale(vec, TRAIL_WIDTH));
			vertices[idx + 1].dif = diffuseColor_;
			vertices[idx + 1].spc = DEF_COLOR;
			vertices[idx + 1].norm = NORM;
			vertices[idx + 1].su = 0.0f;
			vertices[idx + 1].sv = 0.0f;
			vertices[idx + 1].u = (float)idx / (float)(vertices.size());
			vertices[idx + 1].v = 0.0f;

			//ポリゴンのインデックスを次のポリゴンへ
			idx += 2;
		}

		//座標配列のイテレータを次へ
		++it;
	}

	//バックカリング無効化
	SetUseBackCulling(false);
	
	DrawPrimitive3D(vertices.data(), (int)vertices.size(), DX_PRIMTYPE_TRIANGLESTRIP, tex_, true);

	//バックカリング有効化
	SetUseBackCulling(true);
}

void Trail::AddTrailPosList(const VECTOR& addTrailPos)
{
	trailPosList_.emplace_back(addTrailPos);

	int size = (int)trailPosList_.size();


	if (size < TRAIL_LIST_MAX)
	{
		//トレイルの座標が規定の数以下なので終了
		return;
	}

	//最後の座標をポップ
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
		//トレイル用座標の配列が存在しない
		return;
	}

	//最新のトレイル用座標の取得
	const VECTOR& trailEndPos = trailPosList_.back();
	const VECTOR& pos = transformPos_;

	//現在の座標とトレイル用座標の長さを計算
	float length = Math::MagnitudeF(VSub(pos, trailEndPos));

	if (length > TRAIL_LENGTH)
	{
		//トレイル用座標に追加
		AddTrailPosList(transformPos_);
	}
}
