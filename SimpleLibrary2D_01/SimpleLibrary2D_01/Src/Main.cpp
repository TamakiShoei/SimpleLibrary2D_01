#include "Engine/Engine.h"

//メイン関数
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	Engine::SetWindowSize(800, 600);

	if (Engine::Initialize() == false)	//DirectXの初期化
	{
		Engine::Finalize();
		return 0;
	}

	VECTOR lower_left = { -1.0f, -1.0f, 0.0f };
	VECTOR upper_left = { -1.0f, 1.0f, 0.0f };
	VECTOR lower_right = { 1.0f, -1.0f, 0.0f };

	while (Engine::IsClosedWindow() == false)
	{
		Engine::Update();
		//描画
		Engine::ClearScreen();

		Engine::DrawTriangle(lower_left, upper_left, lower_right);
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
