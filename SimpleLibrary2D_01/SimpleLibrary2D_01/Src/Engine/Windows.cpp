#include "Library.h"

LRESULT CALLBACK Library::Windows::WindowProc(HWND hwnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	switch (nMsg)
	{
	case WM_DESTROY:	//終了時
		Library::Engine::Instance()->End();//解放
		PostQuitMessage(0);
		return 0;
	}

	//switch文が処理しなかったメッセージを処理
	return DefWindowProc(hwnd, nMsg, wParam, lParam);
}

HRESULT Library::Windows::InitWindows()
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
	windowRect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
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
		return E_FAIL;
	}

	return S_OK;
}

bool Library::Windows::ProcessMessage()
{
	//ウィンドウの表示
	ShowWindow(FindWindow(WINDOW_TITLE, nullptr), SW_SHOW);

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

	if (isClosed == true)
	{
		return true;
	}
	else
	{
		return false;
	}
}