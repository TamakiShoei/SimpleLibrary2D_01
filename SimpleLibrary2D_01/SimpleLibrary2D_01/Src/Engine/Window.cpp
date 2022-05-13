#include "Window.h"
#include "Engine.h"

LRESULT CALLBACK Window::WindowProc(HWND hwnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	switch (nMsg)
	{
	case WM_DESTROY:	//�I����
		Engine::Instance()->FinalizeEngine();//���
		PostQuitMessage(0);
		return 0;
	}

	//switch�����������Ȃ��������b�Z�[�W������
	return DefWindowProc(hwnd, nMsg, wParam, lParam);
}

HRESULT Window::InitializeWindows()
{
	WNDCLASSEX windowClass;
	RECT windowRect;

	//�E�B���h�E�N���X�̏�����
	windowClass = {};
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = WindowProc;
	windowClass.hInstance = GetModuleHandle(nullptr);
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClass.lpszClassName = WINDOW_CLASS;
	RegisterClassEx(&windowClass);

	//�E�B���h�E�̃T�C�Y�����߂�
	windowRect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

	HWND hwnd;
	//�E�B���h�E�n���h�����쐬
	hwnd = CreateWindow(
		WINDOW_CLASS,
		WINDOW_TITLE,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		nullptr,
		nullptr,
		GetModuleHandle(nullptr),
		nullptr
	);

	if (hwnd == nullptr)
	{
		return E_FAIL;
	}

	//�E�B���h�E�̕\��
	ShowWindow(FindWindow(WINDOW_TITLE, nullptr), SW_SHOW);

	return S_OK;
}

void Window::Update()
{
	MSG msg = {};
	//���C�����[�v
			//�L���[���̃��b�Z�[�W������
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			isClosed = true;
		}
		else
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		isRecievedMessage = true;
	}
	else
	{
		isRecievedMessage = false;
	}
}