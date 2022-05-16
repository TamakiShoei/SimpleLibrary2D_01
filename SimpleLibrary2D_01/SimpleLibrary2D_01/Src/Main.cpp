#include "Engine/Engine.h"

//メイン関数
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	if (Engine::Instance()->Initialize() == false)	//DirectXの初期化
	{
		Engine::Instance()->Finalize();
		return 0;
	}

	while (Engine::Instance()->IsClosedWindow() == false)
	{
		Engine::Instance()->Update();
		//描画
		Engine::Instance()->ClearScreen();

		//------------------------------------------------//
		//				  ここに描画処理					  //
		//------------------------------------------------//

		Engine::Instance()->ScreenFlip();
	}

	Engine::Instance()->Finalize();
	return 0;


	////WM_QUITメッセージの部分をwindowsに返す
	////return static_cast<INT8>(msg.wParam);
}
