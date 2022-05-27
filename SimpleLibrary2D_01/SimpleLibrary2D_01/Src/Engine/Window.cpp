#include "Window.h"
#include "Engine.h"

LRESULT CALLBACK Window::WindowProc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param)
{
	switch (msg)
	{
	case WM_DESTROY:	//終了時
		Engine::Finalize();//解放
		PostQuitMessage(0);
		return 0;
	}

	//switch文が処理しなかったメッセージを処理
	return DefWindowProc(hwnd, msg, w_param, l_param);
}

bool Window::Initialize()
{
	WNDCLASSEX windowClass;
	RECT windowRect;

	//ウィンドウクラスの初期化
	windowClass = {};
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = WindowProc;
	windowClass.hInstance = GetModuleHandle(nullptr);
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClass.lpszClassName = WINDOW_CLASS;
	RegisterClassEx(&windowClass);

	//ウィンドウのサイズを求める
	windowRect = { 0, 0, width, height};
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

	HWND hwnd;
	//ウィンドウハンドルを作成
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
		return false;
	}

	//ウィンドウの表示
	ShowWindow(FindWindow(WINDOW_TITLE, nullptr), SW_SHOW);

	return true;
}

void Window::SetWindowSize(int width_size, int height_size)
{
	width = width_size;
	height = height_size;
}

void Window::Update()
{
	MSG msg = {};
	//メインループ
			//キュー内のメッセージを処理
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