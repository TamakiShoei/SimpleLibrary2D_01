#include "Engine.h"

Engine* Engine::instance = nullptr;

bool Engine::Initialize()
{
	instance = new Engine();

	if (B_FAILED(instance->window.Initialize()))
	{
		return false;
	}
	if (B_FAILED(instance->graphics.Initialize()))
	{
		return false;
	}
	return true;
}

void Engine::Update()
{
	instance->window.Update();
}

void Engine::WaitForPreviousFrame()
{
	const UINT64 fence = instance->graphics.fenceValue;
	instance->graphics.commandQueue->Signal(instance->graphics.fence.Get(), fence);
	instance->graphics.fenceValue++;

	// 前のフレームが終了するまで待機
	if (instance->graphics.fence->GetCompletedValue() < fence) {
		instance->graphics.fence->SetEventOnCompletion(fence, instance->graphics.fenceEvent);
		WaitForSingleObject(instance->graphics.fenceEvent, INFINITE);
	}

	// バックバッファのインデックスを格納
	instance->graphics.frameIndex = instance->graphics.swapChain->GetCurrentBackBufferIndex();
}

void Engine::Finalize()
{
	WaitForPreviousFrame();
	//delete instance;//問題点
}

bool Engine::IsClosedWindow()
{
	return instance->window.IsClosed();
}

void Engine::ClearScreen()
{
	instance->graphics.ClearScreen();
}

void Engine::ScreenFlip()
{
	instance->graphics.ScreenFlip();

	// フレーム後処理
	instance->WaitForPreviousFrame();
}