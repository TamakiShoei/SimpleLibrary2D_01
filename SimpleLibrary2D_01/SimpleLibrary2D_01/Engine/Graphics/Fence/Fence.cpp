#include "Fence.h"

Fence::Fence() : instance(nullptr), fenceEvent(nullptr), fenceValue(0)
{

}

Fence::~Fence()
{
	if (instance != nullptr)
	{
		instance->Release();
	}
}

bool Fence::Initialize(ComPtr<ID3D12Device> device)
{
	// フェンスを作成
	if (FAILED(device.Get()->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(instance.GetAddressOf()))))
	{
		MessageBox(NULL, L"フェンスを作成できませんでした。", WINDOW_TITLE, MB_OK | MB_ICONERROR);
		return false;
	}
	// フェンスとは、同期オブジェクトとしてリソースがGPUにアップロードされるまで待機するもの。

	// フェンス値を1に設定
	fenceValue = 1;

	// フレーム同期に使用するイベントハンドラを作成
	fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	if (fenceEvent == nullptr)
	{
		MessageBox(NULL, L"フェンスイベントハンドラを作成できませんでした。", WINDOW_TITLE, MB_OK | MB_ICONERROR);
		return false;
	}
	return true;
}

void Fence::WaitForPreviousFrame(ComPtr<ID3D12CommandQueue> command_queue)
{
	const UINT64 tmpFence = fenceValue;
	command_queue.Get()->Signal(instance.Get(), tmpFence);
	fenceValue++;

	// 前のフレームが終了するまで待機
	if (instance->GetCompletedValue() < tmpFence) {
		instance->SetEventOnCompletion(tmpFence, fenceEvent);
		WaitForSingleObject(fenceEvent, INFINITE);
	}

}

void Fence::Finalize()
{
	instance->Release();
}