#include "Engine.h"

bool Engine::InitializeEngine()
{
	if (FAILED(window.InitializeWindows()))
	{
		return false;
	}
	if (FAILED(graphics.Initialize()))
	{
		return false;
	}
	return true;
}

void Engine::Update()
{
	window.Update();
}

VOID Engine::WaitForPreviousFrame()
{
	const UINT64 fence = Engine::Instance()->GetGraphics()->fenceValue;
	Engine::Instance()->GetGraphics()->commandQueue->Signal(Engine::Instance()->GetGraphics()->fence.Get(), fence);
	Engine::Instance()->GetGraphics()->fenceValue++;

	// 前のフレームが終了するまで待機
	if (Engine::Instance()->GetGraphics()->fence->GetCompletedValue() < fence) {
		Engine::Instance()->GetGraphics()->fence->SetEventOnCompletion(fence, Engine::Instance()->GetGraphics()->fenceEvent);
		WaitForSingleObject(Engine::Instance()->GetGraphics()->fenceEvent, INFINITE);
	}

	// バックバッファのインデックスを格納
	Engine::Instance()->GetGraphics()->frameIndex = Engine::Instance()->GetGraphics()->swapChain->GetCurrentBackBufferIndex();
}

VOID Engine::OnDestroy()
{
	WaitForPreviousFrame();
	//CloseHandle(GetGraphics()->fenceEvent);
}

void Engine::FinalizeEngine()
{
	OnDestroy();
}

bool Engine::IsClosedWindow()
{
	if (window.IsClosed() == false)
	{
		return false;
	}
	return true;
}

void Engine::ClearScreen()
{
	render.ClearScreen();
}

void Engine::ScreenFlip()
{
	render.ScreenFlip();
}