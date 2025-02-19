#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <Dxlib.h>
#include "Application.h"

/// <summary>
/// WinMain
/// </summary>
int WINAPI WinMain(
	_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{

	//メモリリーク検出用
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	Application::CreateInstace();
	Application& instance = Application::GetInstance();
	

	if (instance.IsInitFail())
	{
		//初期化が失敗
		return -1;
	}

	//実行
	instance.Run();

	//解放
	instance.Destroy();

	return 0;

}