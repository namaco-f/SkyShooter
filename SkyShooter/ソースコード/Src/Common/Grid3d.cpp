
#include "../Utility/MathUtility.h"
#include "Grid3d.h"

//グリッドの長さ
const float LEN = 6000.0f;

//グリッド中心
const float LEN_H = LEN / 2.0f;

//間隔
const float TERM = 50.0f;

//グリッド本数
const int NUM = (int)(LEN / TERM);

//グリッド中間
const int NUM_H = NUM / 2;

const unsigned int COLOR_X = 0xaa0000;
const unsigned int COLOR_Y = 0x00aa00;
const unsigned int COLOR_Z = 0x0000aa;

Grid3d::Grid3d(void)
{
}

Grid3d::~Grid3d(void)
{
}

void Grid3d::Update(void)
{
}

void Grid3d::Draw(void)
{
	unsigned int col;

#pragma region X軸
	col = COLOR_X;
	for (int n = 0; n <= NUM_H; n++)
	{
		VECTOR sPos = { -LEN_H,0.0f,TERM * n };
		VECTOR ePos = { LEN_H,0.0f,TERM * n };

		DrawLine3D(sPos, ePos, col);

		sPos = { -LEN_H,0.0f,-TERM * n };
		ePos = { LEN_H,0.0f,-TERM * n };
		DrawLine3D(sPos, ePos, col);
	}

#pragma endregion

#pragma region Y軸
	col = 0x00aa00;
	//for (int n = 0; n <= NUM_H; n++)
	//{
	//	VECTOR sPos = { -LEN_H,0.0f,TERM * n };
	//	VECTOR ePos = { LEN_H,0.0f,TERM * n };

	//	DrawLine3D(sPos, ePos, col);

	//	sPos = { -LEN_H,0.0f,-TERM * n };
	//	ePos = { LEN_H,0.0f,-TERM * n };
	//	DrawLine3D(sPos, ePos, 0xaa0000);
	//}

	float hPos = TERM * NUM_H / 2.0f;

	VECTOR sPos = { 0.0f ,-LEN_H,0.0f };
	VECTOR ePos = { 0.0f ,LEN_H,0.0f };

	DrawLine3D(sPos, ePos, col);

	

#pragma endregion

#pragma region Z軸
	
	col = 0x0000aa;
	for (int n = 0; n <= NUM_H; n++)
	{
		VECTOR sPos = { TERM * n ,0.0f,-LEN_H };
		VECTOR ePos = { TERM * n,0.0f,LEN_H };

		DrawLine3D(sPos, ePos, col);

		sPos = { -TERM * n,0.0f,- LEN_H };
		ePos = { -TERM * n,0.0f,LEN_H };
		DrawLine3D(sPos, ePos, col);
	}

#pragma endregion



	//DrawLine3D()
}

//void Grid3d::SetLineLength(VECTOR len)
//{
//	length_ = len;
//}
//
//void Grid3d::SetGridSize(VECTOR size)
//{
//	size_ = size;
//}
//
//void Grid3d::SetStartPos(VECTOR pos)
//{
//	pos = pos;
//}
