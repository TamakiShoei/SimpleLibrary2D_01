#include "Graphics.h"

bool Graphics::Initialize()
{
	UINT dxgiFactoryFlags = 0;
	InitializeFactory(dxgiFactoryFlags);

#if defined(_DEBUG)
	//DirectX12のデバッグレイヤーを有効
	{
		ComPtr<ID3D12Debug> debugController;
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(debugController.GetAddressOf()))))
		{
			debugController->EnableDebugLayer();

			//追加のデバッグレイヤーを有効
			dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
		}
	}
#endif

	if (B_FAILED(InitializeAdapter()))
	{
		return false;
	}
	if (B_FAILED(InitializeCommandQueue()))
	{
		return false;
	}
	if (B_FAILED(InitializeSwapChain()))
	{
		return false;
	}

	//Alt+Enterによる全画面遷移をできないようにする
	if (FAILED(factory->MakeWindowAssociation(FindWindow(WINDOW_TITLE, nullptr), DXGI_MWA_NO_ALT_ENTER)))
	{
		MessageBox(NULL, L"画面の設定ができません。", WINDOW_TITLE, MB_OK | MB_ICONERROR);
		return false;
	}

	// ディスクリプタヒープというのは、GPU上に作られるデスクリプタを保存するための配列。
	// GPUメモリ上に存在する、様々なデータやバッファの種類や位置、大きさを示す構造体のようなもの。
	// 何らかのデータ配列として表されているということになる。
	// このように明示的に区切ることによって、その中の構造体のような配列からデータを参照しやすくしている。
	if (B_FAILED(InitializeRtvHeapDesc()))
	{
		return false;
	}
	if (B_FAILED(CreateRenderTargetView()))
	{
		return false;
	}

	// コマンドアロケーターを作成
	if (FAILED(device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(commandAllocator.GetAddressOf()))))
	{
		MessageBox(NULL, L"コマンドアロケータを作成できませんでした。", WINDOW_TITLE, MB_OK | MB_ICONERROR);
		return false;
	}

	// コマンドリストを作成
	if (FAILED(device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator.Get(), nullptr, IID_PPV_ARGS(commandList.GetAddressOf()))))
	{
		MessageBox(NULL, L"コマンドリストを作成できませんでした。", WINDOW_TITLE, MB_OK | MB_ICONERROR);
		return false;
	}

	// コマンドリストをクローズ
	commandList->Close();
	// コマンドリストは記録状態で作成されるが、今回は初期化内でそこに何も入れないのですぐに閉じる。

	if (B_FAILED(InitializeFence()))
	{
		return false;
	}

	return true;
}

void Graphics::ClearScreen()
{
	// コマンドアロケータをリセット
	commandAllocator->Reset();

	// コマンドリストをリセット
	commandList->Reset(commandAllocator.Get(), pipelineState.Get());

	// バックバッファをレンダーターゲットとして使用
	auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(renderTargets[frameIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	commandList->ResourceBarrier(1, &barrier);
	// リソースバリアとは、GPU側で扱うリソースの状況を同期させる機能。
	// マルチスレッドを前提とした動きなので、GPU側の動作も複数のアクセスが同時に行われることを想定した機能だということ。

	// レンダーターゲットビューのハンドルを作成
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(rtvHeap->GetCPUDescriptorHandleForHeapStart(), frameIndex, rtvDescriptorSize);

	// バックバッファに描画(コマンドを記録)
	const FLOAT	clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };		// 青っぽい色
	commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
}

void Graphics::ScreenFlip()
{
	// バックバッファを表示
	auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(renderTargets[frameIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	commandList->ResourceBarrier(1, &barrier);

	// コマンドリストをクローズ
	commandList->Close();

	// コマンドリストを実行
	ID3D12CommandList* ppCommandLists[] = { commandList.Get() };
	commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

	// フレームを最終出力
	swapChain->Present(1, 0);
}

bool Graphics::InitializeFactory(UINT& dxgi_factory_flags)
{
	//ファクトリを作成
	if (FAILED(CreateDXGIFactory2(dxgi_factory_flags, IID_PPV_ARGS(factory.GetAddressOf()))))
	{
		MessageBox(NULL, L"ファクトリを作成できませんでした。", WINDOW_TITLE, MB_OK | MB_ICONERROR);
		return false;
	}

	return true;
}

bool Graphics::InitializeAdapter()
{
	//DirectX12がサポートする利用可能なハードウェアアダプタを検索し取得
	HRESULT hr;
	for (UINT adapterIndex = 0; DXGI_ERROR_NOT_FOUND != factory->EnumAdapters1(adapterIndex, adapter.GetAddressOf()); ++adapterIndex)
	{
		DXGI_ADAPTER_DESC1 adapterDesc;
		adapter->GetDesc1(&adapterDesc);

		//どちらかがFALSEならFALSEでスルー
		if (adapterDesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) continue;

		//アダプタがDirectX12に対応しているか確認
		hr = D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), nullptr);
		if (SUCCEEDED(hr))
		{
			//デバイスを作成
			if (FAILED(D3D12CreateDevice(hardwareAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(Graphics::device.GetAddressOf()))))
			{
				MessageBox(NULL, L"選択したハードウェアデバイスがDIrectX12に対応していません。", WINDOW_TITLE, MB_OK | MB_ICONERROR);
				return false;
			}
			break;
		}
	}

	//関連付け解除
	hardwareAdapter = adapter.Detach();

	//ハードウェアで対応できない場合はWARPデバイス(ソフトウェア)の方を用いる
	if (FAILED(hr))
	{
		ComPtr<IDXGIAdapter> warpAdapter;
		factory->EnumWarpAdapter(IID_PPV_ARGS(warpAdapter.GetAddressOf()));
		if (FAILED(D3D12CreateDevice(warpAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(Graphics::device.GetAddressOf()))))
		{
			MessageBox(NULL, L"選択したWARPデバイスまでもがDirectX12に対応していません。", WINDOW_TITLE, MB_OK | MB_ICONERROR);
			return false;
		}
	}

	return true;
}

bool Graphics::InitializeCommandQueue()
{
	//コマンドキューの設定
	commandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	commandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

	//コマンドキューを作成
	if (FAILED(device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(commandQueue.GetAddressOf()))))
	{
		MessageBox(NULL, L"コマンドキューを作成できませんでした。", WINDOW_TITLE, MB_OK | MB_ICONERROR);
		return false;
	}

	return true;
}

bool Graphics::InitializeSwapChain()
{
	RECT rect;
	GetClientRect(FindWindow(WINDOW_TITLE, nullptr), &rect);

	//スワップチェインの作成
	swapChainDesc.BufferCount = frameCount;
	swapChainDesc.Width = rect.right;
	swapChainDesc.Height = rect.bottom;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.SampleDesc.Count = 1;
	//スワップチェインとは、レンダリング結果を出力するためのオブジェクト
	//紐づいたビデオアダプタやウィンドウに対してレンダリング結果を出力する

	//スワップチェインを作成
	ComPtr<IDXGISwapChain1>	tmpSwapChain;
	if (FAILED(factory->CreateSwapChainForHwnd(commandQueue.Get(), FindWindow(WINDOW_TITLE, nullptr), &swapChainDesc, nullptr, nullptr, tmpSwapChain.GetAddressOf())))
	{
		MessageBox(NULL, L"スワップチェインを作成できませんでした。", WINDOW_TITLE, MB_OK | MB_ICONERROR);
		return false;
	}

	//スワップチェインをキャスト
	tmpSwapChain.As(&swapChain);

	//バックバッファのインデックスを格納
	frameIndex = swapChain->GetCurrentBackBufferIndex();

	return true;
}

bool Graphics::InitializeFence()
{
	// フェンスを作成
	if (FAILED(Graphics::device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(Graphics::fence.GetAddressOf()))))
	{
		MessageBox(NULL, L"フェンスを作成できませんでした。", WINDOW_TITLE, MB_OK | MB_ICONERROR);
		return false;
	}
	// フェンスとは、同期オブジェクトとしてリソースがGPUにアップロードされるまで待機するもの。

	// フェンス値を1に設定
	Graphics::fenceValue = 1;

	// フレーム同期に使用するイベントハンドラを作成
	Graphics::fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	if (Graphics::fenceEvent == nullptr)
	{
		MessageBox(NULL, L"フェンスイベントハンドラを作成できませんでした。", WINDOW_TITLE, MB_OK | MB_ICONERROR);
		return false;
	}
	return true;
}

bool Graphics::InitializeRtvHeapDesc()
{
	//レンダーターゲットビュー用のディスクリプターヒープの設定
	rtvHeapDesc.NumDescriptors = Graphics::frameCount;
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	// ディスクリプタとは、GPUとリソースの橋渡しを行う役割のもの。
	// DX11までは隠蔽されていた。
	// ディスクリプタには3つの概念が存在する。
	// Descriptor：テクスチャなどのリソースをGPUと紐づける。
	// DescriptorHeap：DescriptorHeapからDescriptorを作成する。管理できるDescriptorの種類や数は事前に指定。
	// DescriptorTable：GPU上で使用するDescriptorの数や配置を制御する。DescriptorTableはRootSignatureで設定する。

	// ディスクリプタヒープを作成
	if (FAILED(Graphics::device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(Graphics::rtvHeap.GetAddressOf()))))
	{
		MessageBox(NULL, L"ディスクリプタヒープを作成できませんでした。", WINDOW_TITLE, MB_OK | MB_ICONERROR);
		return false;
	}

	//ディスクリプタのサイズを取得
	Graphics::rtvDescriptorSize = Graphics::device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	return true;
}

bool Graphics::CreateRenderTargetView()
{
	//フレームリソースのハンドルを取得
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(rtvHeap->GetCPUDescriptorHandleForHeapStart());

	// フレームバッファとバックバッファのレンダーターゲットビューを作成
	for (UINT i = 0; i < frameCount; i++)
	{
		// RTVバッファを取得
		if (FAILED(swapChain->GetBuffer(i, IID_PPV_ARGS(renderTargets[i].GetAddressOf()))))
		{
			MessageBox(NULL, L"RTVバッファを取得できませんでした。", WINDOW_TITLE, MB_OK | MB_ICONERROR);
			return false;
		}

		// レンダーターゲットビューを作成
		device->CreateRenderTargetView(renderTargets[i].Get(), nullptr, rtvHandle);

		// ハンドルのオフセット
		rtvHandle.Offset(1, rtvDescriptorSize);
	}

	return true;
}

void Graphics::WaitForPreviousFrame()
{
	const UINT64 tmpFence = fenceValue;
	commandQueue->Signal(fence.Get(), tmpFence);
	IncrementFenceVal();

	// 前のフレームが終了するまで待機
	if (fence->GetCompletedValue() < tmpFence) {
		fence->SetEventOnCompletion(tmpFence, fenceEvent);
		WaitForSingleObject(fenceEvent, INFINITE);
	}

	// バックバッファのインデックスを格納
	frameIndex = swapChain->GetCurrentBackBufferIndex();
}

UINT64 Graphics::GetFenceVal()
{
	return fenceValue;
}

void Graphics::IncrementFenceVal()
{
	fenceValue++;
}
