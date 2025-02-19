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

	//���������[�N���o�p
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	Application::CreateInstace();
	Application& instance = Application::GetInstance();
	

	if (instance.IsInitFail())
	{
		//�����������s
		return -1;
	}

	//���s
	instance.Run();

	//���
	instance.Destroy();

	return 0;

}