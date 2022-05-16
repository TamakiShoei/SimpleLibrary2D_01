#include "Engine.h"

bool Engine::Initialize()
{
	if (B_FAILED(window.Initialize()))
	{
		return false;
	}
	if (B_FAILED(graphics.Initialize()))
	{
		return false;
	}
	return true;
}

void Engine::Update()
{
	window.Update();
}

void Engine::WaitForPreviousFrame()
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

void Engine::OnDestroy()
{
	WaitForPreviousFrame();
	//CloseHandle(GetGraphics()->fenceEvent);
}

void Engine::Finalize()
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
	graphics.ClearScreen();
}

void Engine::ScreenFlip()
{
	graphics.ScreenFlip();

	// フレーム後処理
	Engine::Instance()->WaitForPreviousFrame();
}