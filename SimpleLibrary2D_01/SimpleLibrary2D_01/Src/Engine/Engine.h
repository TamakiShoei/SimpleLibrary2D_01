#ifndef ENGINE_H
#define	ENGINE_H

#include "Graphics.h"
#include "Window.h"

class Engine
{
public:
	static Engine* Instance()
	{
		static Engine instance;
		return &instance;
	}

	bool Initialize();
	void Update();
	void Finalize();

	bool IsClosedWindow();
	void ClearScreen();
	void ScreenFlip();

private:
	//フレーム後処理
	void WaitForPreviousFrame();
	//解放
	void OnDestroy();

	inline Window* GetWindows()
	{
		return &window;
	}

	inline Graphics* GetGraphics()
	{
		return &graphics;
	}

	Window window;
	Graphics graphics;
};


#endif