#include "Graphics.h"

HRESULT Graphics::Initialize()
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

	InitializeAdapter();
	InitializeCommandQueue();
	InitializeSwapChain();

	//Alt+Enterによる全画面遷移をできないようにする
	if (FAILED(factory->MakeWindowAssociation(FindWindow(WINDOW_TITLE, nullptr), DXGI_MWA_NO_ALT_ENTER)))
	{
		MessageBox(NULL, L"画面の設定ができません。", WINDOW_TITLE, MB_OK | MB_ICONERROR);
		return E_FAIL;
	}

	// ディスクリプタヒープというのは、GPU上に作られるデスクリプタを保存するための配列。
	// GPUメモリ上に存在する、様々なデータやバッファの種類や位置、大きさを示す構造体のようなもの。
	// 何らかのデータ配列として表されているということになる。
	// このように明示的に区切ることによって、その中の構造体のような配列からデータを参照しやすくしている。
	InitializeRtvHeapDesc();
	CreateRenderTargetView();

	// コマンドアロケーターを作成
	if (FAILED(device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(commandAllocator.GetAddressOf()))))
	{
		MessageBox(NULL, L"コマンドアロケータを作成できませんでした。", WINDOW_TITLE, MB_OK | MB_ICONERROR);
		return E_FAIL;
	}

	// コマンドリストを作成
	if (FAILED(device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator.Get(), nullptr, IID_PPV_ARGS(commandList.GetAddressOf()))))
	{
		MessageBox(NULL, L"コマンドリストを作成できませんでした。", WINDOW_TITLE, MB_OK | MB_ICONERROR);
		return E_FAIL;
	}

	// コマンドリストをクローズ
	commandList->Close();
	// コマンドリストは記録状態で作成されるが、今回は初期化内でそこに何も入れないのですぐに閉じる。

	InitializeFance();

	return S_OK;
}

HRESULT Graphics::InitializeFactory(UINT& dxgiFactoryFlags)
{
	//ファクトリを作成
	if (FAILED(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(factory.GetAddressOf()))))
	{
		MessageBox(NULL, L"ファクトリを作成できませんでした。", WINDOW_TITLE, MB_OK | MB_ICONERROR);
		return E_FAIL;
	}
}

HRESULT Graphics::InitializeAdapter()
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
		if (FAILED(D3D12CreateDevice(warpAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(Graphics::device.GetAddressOf()))))
		{
			MessageBox(NULL, L"選択したWARPデバイスまでもがDirectX12に対応していません。", WINDOW_TITLE, MB_OK | MB_ICONERROR);
			return E_FAIL;
		}
	}
}

HRESULT Graphics::InitializeCommandQueue()
{
	//コマンドキューの設定
	commandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	commandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

	//コマンドキューを作成
	if (FAILED(device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(commandQueue.GetAddressOf()))))
	{
		MessageBox(NULL, L"コマンドキューを作成できませんでした。", WINDOW_TITLE, MB_OK | MB_ICONERROR);
		return E_FAIL;
	}
}

HRESULT Graphics::InitializeSwapChain()
{
	//スワップチェインの作成
	swapChainDesc.BufferCount = frameCount;
	swapChainDesc.Width = WINDOW_WIDTH;
	swapChainDesc.Height = WINDOW_HEIGHT;
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
		return E_FAIL;
	}

	//スワップチェインをキャスト
	tmpSwapChain.As(&swapChain);

	//バックバッファのインデックスを格納
	frameIndex = swapChain->GetCurrentBackBufferIndex();
}

HRESULT Graphics::InitializeFance()
{
	// フェンスを作成
	if (FAILED(Graphics::device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(Graphics::fence.GetAddressOf()))))
	{
		MessageBox(NULL, L"フェンスを作成できませんでした。", WINDOW_TITLE, MB_OK | MB_ICONERROR);
		return E_FAIL;
	}
	// フェンスとは、同期オブジェクトとしてリソースがGPUにアップロードされるまで待機するもの。

	// フェンス値を1に設定
	Graphics::fenceValue = 1;

	// フレーム同期に使用するイベントハンドラを作成
	Graphics::fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	if (Graphics::fenceEvent == nullptr)
	{
		MessageBox(NULL, L"フェンスイベントハンドラを作成できませんでした。", WINDOW_TITLE, MB_OK | MB_ICONERROR);
		return E_FAIL;
	}
	// 待つ際にWindowsのEventを利用
}

HRESULT Graphics::InitializeRtvHeapDesc()
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
		return E_FAIL;
	}

	//ディスクリプタのサイズを取得
	Graphics::rtvDescriptorSize = Graphics::device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

}

HRESULT Graphics::CreateRenderTargetView()
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
			return E_FAIL;
		}

		// レンダーターゲットビューを作成
		device->CreateRenderTargetView(renderTargets[i].Get(), nullptr, rtvHandle);

		// ハンドルのオフセット
		rtvHandle.Offset(1, rtvDescriptorSize);
	}
}
