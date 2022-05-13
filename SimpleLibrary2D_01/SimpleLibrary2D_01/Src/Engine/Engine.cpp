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

	// �O�̃t���[�����I������܂őҋ@
	if (Engine::Instance()->GetGraphics()->fence->GetCompletedValue() < fence) {
		Engine::Instance()->GetGraphics()->fence->SetEventOnCompletion(fence, Engine::Instance()->GetGraphics()->fenceEvent);
		WaitForSingleObject(Engine::Instance()->GetGraphics()->fenceEvent, INFINITE);
	}

	// �o�b�N�o�b�t�@�̃C���f�b�N�X���i�[
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