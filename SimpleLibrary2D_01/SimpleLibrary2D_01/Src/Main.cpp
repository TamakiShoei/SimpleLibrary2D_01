#include "Engine/Library.h"

//���C���֐�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	if (FAILED(Library::Engine::Instance()->OnInit()))	//DirectX�̏�����
	{
		Library::Engine::Instance()->End();
		return 0;
	}

	while (Library::Engine::Instance()->GetWindows()->ProcessMessage() == false)
	{
		//�`��
		Library::Render::Instance()->ClearScreen();
		Library::Render::Instance()->ScreenFlip();
	}

	//Library::Engine::Instance()->End();
	return 0;


	////WM_QUIT���b�Z�[�W�̕�����windows�ɕԂ�
	////return static_cast<INT8>(msg.wParam);
}
