#include "Render.h"
#include "Engine.h"

void Render::ClearScreen()
{
	// コマンドアロケータをリセット
	Engine::Instance()->GetGraphics()->commandAllocator->Reset();

	// コマンドリストをリセット
	Engine::Instance()->GetGraphics()->commandList->Reset(Engine::Instance()->GetGraphics()->commandAllocator.Get(), Engine::Instance()->GetGraphics()->pipelineState.Get());

	// バックバッファをレンダーターゲットとして使用
	auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(Engine::Instance()->GetGraphics()->renderTargets[Engine::Instance()->GetGraphics()->frameIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	Engine::Instance()->GetGraphics()->commandList->ResourceBarrier(1, &barrier);
	// リソースバリアとは、GPU側で扱うリソースの状況を同期させる機能。
	// マルチスレッドを前提とした動きなので、GPU側の動作も複数のアクセスが同時に行われることを想定した機能だということ。

	// レンダーターゲットビューのハンドルを作成
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(Engine::Instance()->GetGraphics()->rtvHeap->GetCPUDescriptorHandleForHeapStart(), Engine::Instance()->GetGraphics()->frameIndex, Engine::Instance()->GetGraphics()->rtvDescriptorSize);

	// バックバッファに描画(コマンドを記録)
	const FLOAT	clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };		// 青っぽい色
	Engine::Instance()->GetGraphics()->commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
}

void Render::ScreenFlip()
{
	// バックバッファを表示
	auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(Engine::Instance()->GetGraphics()->renderTargets[Engine::Instance()->GetGraphics()->frameIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	Engine::Instance()->GetGraphics()->commandList->ResourceBarrier(1, &barrier);

	// コマンドリストをクローズ
	Engine::Instance()->GetGraphics()->commandList->Close();

	// コマンドリストを実行
	ID3D12CommandList* ppCommandLists[] = { Engine::Instance()->GetGraphics()->commandList.Get() };
	Engine::Instance()->GetGraphics()->commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

	// フレームを最終出力
	Engine::Instance()->GetGraphics()->swapChain->Present(1, 0);

	// フレーム後処理
	Engine::Instance()->WaitForPreviousFrame();
}
