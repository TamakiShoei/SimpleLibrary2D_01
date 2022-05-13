#ifndef ENGINE_H
#define	ENGINE_H

#include "Graphics.h"
#include "Window.h"
#include "Render.h"

class Engine
{
public:
	static Engine* Instance()
	{
		static Engine instance;
		return &instance;
	}

	bool InitializeEngine();
	void Update();
	void FinalizeEngine();

	bool IsClosedWindow();
	void ClearScreen();
	void ScreenFlip();

	//ƒtƒŒ[ƒ€Œãˆ—
	VOID WaitForPreviousFrame();
	//‰ğ•ú
	VOID OnDestroy();

	Window* GetWindows()
	{
		return &window;
	}

	Render* GetRender()
	{
		return &render;
	}

	Graphics* GetGraphics()
	{
		return &graphics;
	}

	Window window;
	Render render;
	Graphics graphics;
};


#endif