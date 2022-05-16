#ifndef WINDOW_H
#define WINDOW_H

#include "CommonDifinition.h"

class Window
{
public:
	Window() : isClosed(false), isRecievedMessage(false)
	{
	}

	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT n_msg, WPARAM w_param, LPARAM l_param);

	bool Initialize();

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