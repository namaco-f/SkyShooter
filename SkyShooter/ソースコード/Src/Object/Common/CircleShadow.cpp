
#include <vector>
#include "../Manager/ResourceManager/ResourceManager.h"
#include "../Manager/ResourceManager/Image/ImageResourcesManager.h"
#include "../Utility/MathUtility.h"
#include "../Common/Geometry2D.h"
#include "CircleShadow.h"

using namespace std;

const int POLYGON_NUM = 2;
const int TRIANGLE_VERTEX_NUM = 3;
const COLOR_U8 DEF_DIFFUSE = { 255,255,255,255 };
const COLOR_U8 DEF_SPEC = { 255,255,255,255 };
const VECTOR DEF_NORM = { 0.0f,1.0f,0.0f };
const float SCALE_HALF = 0.5f;


CircleShadow::CircleShadow(
	const Transform& trans,
	const VECTOR& polySize,
	const float offSetY)
	:
	circleShadowHandle_(-1),
	transPos_(trans.pos),
	quaRot_(GetQuaRotY(trans.quaRot)),
	polySize_(polySize),
	polySizeHalf_(VScale(polySize_, SCALE_HALF)),
	offSetY_(offSetY),
	vertices_{},
	index_{}
{
	auto& resMngImg = ResourceManager::GetInstance().GetImageResourcesInstance();

	//ポリゴン配列
	vertices_.reserve((size_t)(POLYGON_NUM * TRIANGLE_VERTEX_NUM));

	//インデックス配列
	index_.reserve((size_t)(POLYGON_NUM * TRIANGLE_VERTEX_NUM));
	
	MakeVertices();

	vertices_.resize(vertices_.size());
	index_.resize(index_.size());

	circleShadowHandle_ = resMngImg.Load(SRC_IMAGE::CircleShadow).GetHandleId();
}

CircleShadow::~CircleShadow(void)
{
}

void CircleShadow::Init(void)
{
}

void CircleShadow::Update(const Transform& trans)
{
	transPos_ = trans.pos;
	VECTOR degRot = trans.quaRot.ToEuler();
	quaRot_ = Quaternion::Euler({ 0.0f,degRot.y ,0.0f });
	UpdateVertices();
}

void CircleShadow::Draw(void)
{
	DrawPrimitiveIndexed3D(
		vertices_.data(), (int)vertices_.size(),
		index_.data(), (int)index_.size(),
		DX_PRIMTYPE_TRIANGLESTRIP, circleShadowHandle_, true
	);
}

const VECTOR CircleShadow::GetPolyPos(const VECTOR& size)
{
	return VECTOR();
}

void CircleShadow::MakeVertices(void)
{
	VERTEX3D vertex = VERTEX3D();

	VECTOR size = {};

	//左上のポリゴンを生成
	size = polySizeHalf_;
	size.x *= -1.0f;

	vertex.pos.y += offSetY_;
	vertex.dif = DEF_DIFFUSE;
	vertex.spc = DEF_SPEC;
	vertex.norm = DEF_NORM;
	vertex.su = 0.0f;
	vertex.sv = 0.0f;
	vertex.u = 0.0f;
	vertex.v = 0.0f;
	vertices_.emplace_back(vertex);

	//右上のポリゴンを生成
	size = polySizeHalf_;

	vertex.pos = VAdd(transPos_, size);
	vertex.pos.y += offSetY_;
	vertex.u = 1.0f;
	vertex.v = 0.0f;
	vertices_.emplace_back(vertex);

	//左下のポリゴンを生成
	size = VScale(polySizeHalf_, -1.0f);

	vertex.pos = VAdd(transPos_, size);
	vertex.pos.y += offSetY_;
	vertex.u = 0.0f;
	vertex.v = 1.0f;
	vertices_.emplace_back(vertex);

	//右下のポリゴンを生成
	size = polySizeHalf_;
	size.z *= -1.0f;

	vertex.pos.y += offSetY_;
	vertex.u = 1.0f;
	vertex.v = 1.0f;
	vertices_.emplace_back(vertex);

	//2ポリゴンのインデックスデータをセット
	index_.emplace_back(0);
	index_.emplace_back(1);
	index_.emplace_back(2);

	index_.emplace_back(3);
	index_.emplace_back(index_[2]);
	index_.emplace_back(index_[1]);
}

void CircleShadow::UpdateVertices(void)
{
	int idx = 0;
	VECTOR size = {};

	//左上のポリゴンを更新
	size = polySizeHalf_;
	size.x *= -1.0f;
	vertices_[idx].pos = VAdd(transPos_, GetRotPos(size));
	vertices_[idx].pos.y += offSetY_;

	//右上のポリゴンを更新
	size = polySizeHalf_;
	vertices_[++idx].pos = VAdd(transPos_, GetRotPos(size));
	vertices_[idx].pos.y += offSetY_;

	//左下のポリゴンを更新
	size = VScale(polySizeHalf_, -1.0f);
	vertices_[++idx].pos = VAdd(transPos_, GetRotPos(size));
	vertices_[idx].pos.y += offSetY_;


	//右下のポリゴンを更新
	size = polySizeHalf_;
	size.z *= -1.0f;
	vertices_[++idx].pos = VAdd(transPos_, GetRotPos(size));
	vertices_[idx].pos.y += offSetY_;
}

const Quaternion CircleShadow::GetQuaRotY(const Quaternion& qua)
{
	VECTOR degRot = qua.ToEuler();

	return Quaternion::Euler({ 0.0f,degRot.y ,0.0f });
}

const VECTOR CircleShadow::GetRotPos(const VECTOR size)
{
	return quaRot_.PosAxis(size);
}
