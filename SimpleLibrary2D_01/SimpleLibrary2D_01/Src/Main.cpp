#include "Engine/Library.h"

//メイン関数
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	if (FAILED(Library::Engine::Instance()->OnInit()))	//DirectXの初期化
	{
		Library::Engine::Instance()->End();
		return 0;
	}

	while (Library::Engine::Instance()->GetWindows()->ProcessMessage() == false)
	{
		//描画
		Library::Engine::Instance()->GetRender()->ClearScreen();
		Library::Engine::Instance()->GetRender()->ScreenFlip();
	}

	//Library::Engine::Instance()->End();
	return 0;


	////WM_QUITメッセージの部分をwindowsに返す
	////return static_cast<INT8>(msg.wParam);
}
