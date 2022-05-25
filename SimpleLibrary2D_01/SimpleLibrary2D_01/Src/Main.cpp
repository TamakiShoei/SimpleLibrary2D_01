#include "Engine/Engine.h"

//メイン関数
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	if (Engine::Initialize() == false)	//DirectXの初期化
	{
		Engine::Finalize();
		return 0;
	}

	while (Engine::IsClosedWindow() == false)
	{
		Engine::Update();
		//描画
		Engine::ClearScreen();

		//------------------------------------------------//
		//				  ここに描画処理					  //
		//------------------------------------------------//

		Engine::ScreenFlip();
	}

	Engine::Finalize();
	return 0;


	////WM_QUITメッセージの部分をwindowsに返す
	////return static_cast<INT8>(msg.wParam);
}
