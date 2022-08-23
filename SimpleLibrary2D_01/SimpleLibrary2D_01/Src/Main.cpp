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

	int counter = 0;

	while (Engine::IsClosedWindow() == false)
	{
		Engine::Update();

		Engine::ClearScreen();

		Engine::DrawTexture(0.0f, 0.0f, "Res/Texture/Test4.jpg");

		counter++;
		Engine::DrawTexture(counter, 100.0f, "Res/Texture/Test3.png");

		Engine::ScreenFlip();
	}

	Engine::Finalize();
	return 0;
}
