#include "Library.h"

HRESULT Library::Engine::OnInit()
{
	if (FAILED(Library::Engine::Instance()->GetWindows()->InitWindows()))
	{
		return E_FAIL;
	}

	UINT dxgiFactoryFlags = 0;
	InitFactory(dxgiFactoryFlags);

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

	InitAdapter();
	InitCommandQueue();

	//Alt+Enterによる全画面遷移をできないようにする
	if (FAILED(factory->MakeWindowAssociation(FindWindow(WINDOW_TITLE, nullptr), DXGI_MWA_NO_ALT_ENTER)))
	{
		MessageBox(NULL, L"画面の設定ができません。", WINDOW_TITLE, MB_OK | MB_ICONERROR);
		return E_FAIL;
	}

	//スワップチェインの作成
	swapChainDesc.BufferCount = Engine::Instance()->frameCount;
	swapChainDesc.Width = WINDOW_WIDTH;
	swapChainDesc.Height = WINDOW_HEIGHT;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.SampleDesc.Count = 1;
	//スワップチェインとは、レンダリング結果を出力するためのオブジェクト
	//紐づいたビデオアダプタやウィンドウに対してレンダリング結果を出力する

	//スワップチェインを作成
	ComPtr<IDXGISwapChain1> swapChain;
	if (FAILED(factory->CreateSwapChainForHwnd(Engine::Instance()->commandQueue.Get(), FindWindow(WINDOW_TITLE, nullptr), &swapChainDesc, nullptr, nullptr, swapChain.GetAddressOf())))
	{
		MessageBox(NULL, L"スワップチェインを作成できませんでした。", WINDOW_TITLE, MB_OK | MB_ICONERROR);
		return E_FAIL;
	}

	//スワップチェインをキャスト
	swapChain.As(&Engine::Instance()->swapChain);

	//バックバッファのインデックスを格納
	Engine::Instance()->frameIndex = Engine::Instance()->swapChain->GetCurrentBackBufferIndex();

	// ディスクリプタヒープというのは、GPU上に作られるデスクリプタを保存するための配列。
	// GPUメモリ上に存在する、様々なデータやバッファの種類や位置、大きさを示す構造体のようなもの。
	// 何らかのデータ配列として表されているということになる。
	// このように明示的に区切ることによって、その中の構造体のような配列からデータを参照しやすくしている。
	InitRtvHeapDesc();

	{
		//フレームリソースのハンドルを取得
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(Engine::Instance()->rtvHeap->GetCPUDescriptorHandleForHeapStart());

		// フレームバッファとバックバッファのレンダーターゲットビューを作成
		for (UINT i = 0; i < Engine::Instance()->frameCount; i++)
		{
			// RTVバッファを取得
		if (FAILED(Engine::Instance()->swapChain->GetBuffer(i, IID_PPV_ARGS(Engine::Instance()->renderTargets[i].GetAddressOf()))))
			{
				MessageBox(NULL, L"RTVバッファを取得できませんでした。", WINDOW_TITLE, MB_OK | MB_ICONERROR);
				return E_FAIL;
			}

			// レンダーターゲットビューを作成
			Engine::Instance()->device->CreateRenderTargetView(Engine::Instance()->renderTargets[i].Get(), nullptr, rtvHandle);

			// ハンドルのオフセット
			rtvHandle.Offset(1, Engine::Instance()->rtvDescriptorSize);
		}
	}

	// コマンドアロケーターを作成
	if (FAILED(Engine::Instance()->device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(Engine::Instance()->commandAllocator.GetAddressOf()))))
	{
		MessageBox(NULL, L"コマンドアロケータを作成できませんでした。", WINDOW_TITLE, MB_OK | MB_ICONERROR);
		return E_FAIL;
	}

	// コマンドリストを作成
	if (FAILED(Engine::Instance()->device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, Engine::Instance()->commandAllocator.Get(), nullptr, IID_PPV_ARGS(Engine::Instance()->commandList.GetAddressOf()))))
	{
		MessageBox(NULL, L"コマンドリストを作成できませんでした。", WINDOW_TITLE, MB_OK | MB_ICONERROR);
		return E_FAIL;
	}

	// コマンドリストをクローズ
	Engine::Instance()->commandList->Close();
	// コマンドリストは記録状態で作成されるが、今回は初期化内でそこに何も入れないのですぐに閉じる。

	InitFance();

	return S_OK;
}



VOID Library::Render::ClearScreen()
{
	// コマンドアロケータをリセット
	Library::Engine::Instance()->commandAllocator->Reset();

	// コマンドリストをリセット
	Engine::Instance()->commandList->Reset(Engine::Instance()->commandAllocator.Get(), Engine::Instance()->pipelineState.Get());

	// バックバッファをレンダーターゲットとして使用
	auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(Engine::Instance()->renderTargets[Engine::Instance()->frameIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	Engine::Instance()->commandList->ResourceBarrier(1, &barrier);
	// リソースバリアとは、GPU側で扱うリソースの状況を同期させる機能。
	// マルチスレッドを前提とした動きなので、GPU側の動作も複数のアクセスが同時に行われることを想定した機能だということ。

	// レンダーターゲットビューのハンドルを作成
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(Engine::Instance()->rtvHeap->GetCPUDescriptorHandleForHeapStart(), Engine::Instance()->frameIndex, Engine::Instance()->rtvDescriptorSize);

	// バックバッファに描画(コマンドを記録)
	const FLOAT	clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };		// 青っぽい色
	Engine::Instance()->commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
}

VOID Library::Render::ScreenFlip()
{
	// バックバッファを表示
	auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(Engine::Instance()->renderTargets[Engine::Instance()->frameIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	Engine::Instance()->commandList->ResourceBarrier(1, &barrier);

	// コマンドリストをクローズ
	Engine::Instance()->commandList->Close();

	// コマンドリストを実行
	ID3D12CommandList* ppCommandLists[] = { Engine::Instance()->commandList.Get() };
	Engine::Instance()->commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

	// フレームを最終出力
	Engine::Instance()->swapChain->Present(1, 0);

	// フレーム後処理
	Library::Engine::Instance()->WaitForPreviousFrame();
}

VOID Library::Engine::WaitForPreviousFrame()
{
	const UINT64 fence = Engine::Instance()->fenceValue;
	Engine::Instance()->commandQueue->Signal(Engine::Instance()->fence.Get(), fence);
	Engine::Instance()->fenceValue++;

	// 前のフレームが終了するまで待機
	if (Engine::Instance()->fence->GetCompletedValue() < fence) {
		Engine::Instance()->fence->SetEventOnCompletion(fence, Engine::Instance()->fenceEvent);
		WaitForSingleObject(Engine::Instance()->fenceEvent, INFINITE);
	}

	// バックバッファのインデックスを格納
	Engine::Instance()->frameIndex = Engine::Instance()->swapChain->GetCurrentBackBufferIndex();
}

VOID Library::Engine::OnDestroy()
{
	WaitForPreviousFrame();
	CloseHandle(Engine::Instance()->fenceEvent);
}

HRESULT Library::Engine::InitFactory(UINT &dxgiFactoryFlags)
{
	//ファクトリを作成
	if (FAILED(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(factory.GetAddressOf()))))
	{
		MessageBox(NULL, L"ファクトリを作成できませんでした。", WINDOW_TITLE, MB_OK | MB_ICONERROR);
		return E_FAIL;
	}
}

HRESULT Library::Engine::InitAdapter()
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
			if (FAILED(D3D12CreateDevice(hardwareAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(Engine::Instance()->device.GetAddressOf()))))
			{
				MessageBox(NULL, L"選択したハードウェアデバイスがDIrectX12に対応していません。", WINDOW_TITLE, MB_OK | MB_ICONERROR);
				return E_FAIL;
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
		if (FAILED(D3D12CreateDevice(warpAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(Engine::Instance()->device.GetAddressOf()))))
		{
			MessageBox(NULL, L"選択したWARPデバイスまでもがDirectX12に対応していません。", WINDOW_TITLE, MB_OK | MB_ICONERROR);
			return E_FAIL;
		}
	}
}

HRESULT Library::Engine::InitCommandQueue()
{
	//コマンドキューの設定
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

	//コマンドキューを作成
	if (FAILED(Engine::Instance()->device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(Engine::Instance()->commandQueue.GetAddressOf()))))
	{
		MessageBox(NULL, L"コマンドキューを作成できませんでした。", WINDOW_TITLE, MB_OK | MB_ICONERROR);
		return E_FAIL;
	}
}

HRESULT Library::Engine::InitFance()
{
	// フェンスを作成
	if (FAILED(Engine::Instance()->device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(Engine::Instance()->fence.GetAddressOf()))))
	{
		MessageBox(NULL, L"フェンスを作成できませんでした。", WINDOW_TITLE, MB_OK | MB_ICONERROR);
		return E_FAIL;
	}
	// フェンスとは、同期オブジェクトとしてリソースがGPUにアップロードされるまで待機するもの。

	// フェンス値を1に設定
	Engine::Instance()->fenceValue = 1;

	// フレーム同期に使用するイベントハンドラを作成
	Engine::Instance()->fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	if (Engine::Instance()->fenceEvent == nullptr)
	{
		MessageBox(NULL, L"フェンスイベントハンドラを作成できませんでした。", WINDOW_TITLE, MB_OK | MB_ICONERROR);
		return E_FAIL;
	}
	// 待つ際にWindowsのEventを利用
}

HRESULT Library::Engine::InitRtvHeapDesc()
{
	//レンダーターゲットビュー用のディスクリプターヒープの設定
	rtvHeapDesc.NumDescriptors = Engine::Instance()->frameCount;
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	// ディスクリプタとは、GPUとリソースの橋渡しを行う役割のもの。
	// DX11までは隠蔽されていた。
	// ディスクリプタには3つの概念が存在する。
	// Descriptor：テクスチャなどのリソースをGPUと紐づける。
	// DescriptorHeap：DescriptorHeapからDescriptorを作成する。管理できるDescriptorの種類や数は事前に指定。
	// DescriptorTable：GPU上で使用するDescriptorの数や配置を制御する。DescriptorTableはRootSignatureで設定する。

	// ディスクリプタヒープを作成
	if (FAILED(Engine::Instance()->device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(Engine::Instance()->rtvHeap.GetAddressOf()))))
	{
		MessageBox(NULL, L"ディスクリプタヒープを作成できませんでした。", WINDOW_TITLE, MB_OK | MB_ICONERROR);
		return E_FAIL;
	}

	//ディスクリプタのサイズを取得
	Engine::Instance()->rtvDescriptorSize = Engine::Instance()->device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

}

void Library::Engine::End()
{
	OnDestroy();
}