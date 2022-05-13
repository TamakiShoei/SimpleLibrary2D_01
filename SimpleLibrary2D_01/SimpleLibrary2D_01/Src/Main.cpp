#include "Engine/Engine.h"

//���C���֐�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	if (Engine::Instance()->InitializeEngine() == false)	//DirectX�̏�����
	{
		Engine::Instance()->FinalizeEngine();
		return 0;
	}

	while (Engine::Instance()->IsClosedWindow() == false)
	{
		Engine::Instance()->Update();
		//�`��
		Engine::Instance()->ClearScreen();

		//------------------------------------------------//
		//				  �����ɕ`�揈��					  //
		//------------------------------------------------//

		Engine::Instance()->ScreenFlip();
	}

	Engine::Instance()->FinalizeEngine();
	return 0;


	////WM_QUIT���b�Z�[�W�̕�����windows�ɕԂ�
	////return static_cast<INT8>(msg.wParam);
}
