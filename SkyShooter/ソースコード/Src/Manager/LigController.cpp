#include "../Utility/MathUtility.h"
#include "LigController.h"

const VECTOR DEF_LIG_DIR = { 0.3f, -0.7f, 0.8f };

LigController::LigController(void)
	: 
	ligInfo_(LightInfo())
{

	// ライトの設定
	SetUseLighting(true);

	ligInfo_.dir = DEF_LIG_DIR;
	ligInfo_.pos = GetLightPosition();
	
	// ライトの設定
	ChangeLightTypeDir(ligInfo_.dir);

}

LigController::~LigController(void)
{
}

void LigController::Init(void)
{
}

void LigController::Update(void)
{
}

void LigController::SetLigColor(const VECTOR& col)
{
	ligInfo_.col = col;

	COLOR_F colorL = { ligInfo_.col.x,ligInfo_.col.y ,ligInfo_.col.z,1.0f };
	SetLightDifColor(colorL);
}

const VECTOR& LigController::GetLigColor(void) const
{
	return ligInfo_.col;
}

void LigController::SetLigDir(const VECTOR& dir)
{
	ligInfo_.dir = dir;
	SetLightDirection(ligInfo_.dir);
}

const VECTOR LigController::GetLigDir(void) const
{
	return GetLightDirection();
}

void LigController::SetLigPos(const VECTOR& pos)
{
	ligInfo_.pos = pos;
	SetLightPosition(ligInfo_.pos);
}

const VECTOR& LigController::GetLigPos()
{
	return ligInfo_.pos;
}


