#ifndef WINDOW_H
#define WINDOW_H

#include "CommonDifinition.h"

class Window
{
public:
	Window() : isClosed(false), isRecievedMessage(false)
	{
	}

	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT nMsg, WPARAM wParam, LPARAM lParam);

	HRESULT InitializeWindows();

	void Update();

	bool IsClosed()
	{
		return isClosed;
	}

	/*bool IsRecievedMessage()
	{
	return isRecievedMessage;
	}*/

private:
	bool isClosed;
	bool isRecievedMessage;
};

#endif