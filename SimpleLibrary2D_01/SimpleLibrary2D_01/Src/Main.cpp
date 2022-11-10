#include "../Engine/Engine.h"

float posx;

//メイン関数
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	Engine::SetWindowSize(800, 600);

	if (Engine::Initialize() == false)	//DirectXの初期化
	{
		Engine::Finalize();
		return 0;
	}

	int handle = Engine::LoadTexture("Res/Texture/Test4.jpg");
	int handle2 = Engine::LoadTexture("Res/Texture/Test3.png");
	int handle3 = Engine::LoadTexture("Res/Texture/Test.png");

	while (Engine::IsClosedWindow() == false)
	{
		Engine::Update();

		if (Engine::IsPushedKey(DIK_W))
		{
			posx += 30;
		}

		Engine::ClearScreen();

		Engine::DrawTexture(posx, 0.0f, handle);
		Engine::DrawTexture(0.0f, 100.0f, handle2);
		//Engine::DrawTexture(400.0f, 400.0f, handle);

		Engine::ScreenFlip();
	}

	Engine::Finalize();
	return 0;
}
