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
		Library::Engine::Instance()->GetRender()->ClearScreen();
		Library::Engine::Instance()->GetRender()->ScreenFlip();
	}

	//Library::Engine::Instance()->End();
	return 0;


	////WM_QUIT���b�Z�[�W�̕�����windows�ɕԂ�
	////return static_cast<INT8>(msg.wParam);
}
