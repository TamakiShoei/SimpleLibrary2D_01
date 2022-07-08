#include "Graphics.h"
#include "DirectXMath.h"
#include <d3dcompiler.h>

#pragma comment(lib, "d3dcompiler.lib")

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
	if (B_FAILED(CreatePipeline()))
	{
		return false;
	}
	if (B_FAILED(CreateRtvDescHeap()))
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

	//ビューポートとシザーの設定
	SetDrawArea();

	commandList->Close();
	// コマンドリストは記録状態で作成されるが、今回は初期化内でそこに何も入れないのですぐに閉じる。

	if (B_FAILED(InitializeFence()))
	{
		return false;
	}

	return true;
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
			if (FAILED(D3D12CreateDevice(hardwareAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(device.GetAddressOf()))))
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
		if (FAILED(D3D12CreateDevice(warpAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(device.GetAddressOf()))))
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
	if (FAILED(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(fence.GetAddressOf()))))
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

bool Graphics::CreateRtvDescHeap()
{
	//レンダーターゲットビュー用のディスクリプターヒープの設定
	rtvHeapDesc.NumDescriptors = frameCount;
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	// ディスクリプタとは、GPUとリソースの橋渡しを行う役割のもの。
	// DX11までは隠蔽されていた。
	// ディスクリプタには3つの概念が存在する。
	// Descriptor：テクスチャなどのリソースをGPUと紐づける。
	// DescriptorHeap：DescriptorHeapからDescriptorを作成する。管理できるDescriptorの種類や数は事前に指定。
	// DescriptorTable：GPU上で使用するDescriptorの数や配置を制御する。DescriptorTableはRootSignatureで設定する。

	// ディスクリプタヒープを作成
	if (FAILED(device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(rtvHeap.GetAddressOf()))))
	{
		MessageBox(NULL, L"ディスクリプタヒープを作成できませんでした。", WINDOW_TITLE, MB_OK | MB_ICONERROR);
		return false;
	}

	//ディスクリプタのサイズを取得
	rtvDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

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

void Graphics::SetDrawArea()
{
	RECT rect;
	if (B_FAILED(GetClientRect(FindWindow(WINDOW_TITLE, nullptr), &rect)))
	{
		return;
	}

	viewport.Width = rect.right;
	viewport.Height = rect.bottom;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MaxDepth = 1.0f;
	viewport.MinDepth = 0.0f;

	scissorRect.top = 0;
	scissorRect.left = 0;
	scissorRect.right = scissorRect.left + rect.right;
	scissorRect.bottom = scissorRect.top + rect.bottom;
}

//bool Graphics::CreateVertexResource()
//{
//
//}

bool Graphics::CreatePipeline()
{
	ID3DBlob* vsBlob = nullptr;
	ID3DBlob* psBlob = nullptr;
	ID3DBlob* rootSigBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;

	if (FAILED(D3DCompileFromFile(
		L"Res/Shader/VertexShader.hlsl",
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"VS", "vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&vsBlob, &errorBlob)))
	{
		return false;
	}

	if (FAILED(D3DCompileFromFile(
		L"Res/Shader/PixelShader.hlsl",
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"PS", "ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&psBlob, &errorBlob)))
	{
		return false;
	}

	D3D12_INPUT_ELEMENT_DESC inputLayout[] =
	{
		{
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};

	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};

	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	if (FAILED(D3D12SerializeRootSignature(
		&rootSignatureDesc,
		D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob)))
	{
		return false;
	}

	device->CreateRootSignature(
		0,
		rootSigBlob->GetBufferPointer(),
		rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature));

	D3D12_RENDER_TARGET_BLEND_DESC renderTargetBlendDesc = {};
	renderTargetBlendDesc.BlendEnable = false;
	renderTargetBlendDesc.LogicOpEnable = false;
	renderTargetBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipeline = {};
	graphicsPipeline.pRootSignature = rootSignature.Get();
	graphicsPipeline.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	graphicsPipeline.VS.BytecodeLength = vsBlob->GetBufferSize();
	graphicsPipeline.PS.pShaderBytecode = psBlob->GetBufferPointer();
	graphicsPipeline.PS.BytecodeLength = psBlob->GetBufferSize();
	graphicsPipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	graphicsPipeline.RasterizerState.MultisampleEnable = false;
	graphicsPipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	graphicsPipeline.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
	graphicsPipeline.RasterizerState.DepthClipEnable = true;
	graphicsPipeline.BlendState.AlphaToCoverageEnable = false;
	graphicsPipeline.BlendState.IndependentBlendEnable = false;
	graphicsPipeline.BlendState.RenderTarget[0] = renderTargetBlendDesc;
	graphicsPipeline.InputLayout.pInputElementDescs = inputLayout;
	graphicsPipeline.InputLayout.NumElements = _countof(inputLayout);
	graphicsPipeline.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
	graphicsPipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	graphicsPipeline.NumRenderTargets = 1;
	graphicsPipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	graphicsPipeline.SampleDesc.Count = 1;
	graphicsPipeline.SampleDesc.Quality = 0;

	if (FAILED(device->CreateGraphicsPipelineState(
		&graphicsPipeline, IID_PPV_ARGS(pipelineState.GetAddressOf()))))
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

	//ビューポートのセット
	commandList->RSSetViewports(1, &viewport);
	//シザーのセット
	commandList->RSSetScissorRects(1, &scissorRect);

	// バックバッファのインデックスを格納
	frameIndex = swapChain->GetCurrentBackBufferIndex();

	// バックバッファをレンダーターゲットとして使用
	auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(renderTargets[frameIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	commandList->ResourceBarrier(1, &barrier);
	// リソースバリアとは、GPU側で扱うリソースの状況を同期させる機能。
	// マルチスレッドを前提とした動きなので、GPU側の動作も複数のアクセスが同時に行われることを想定した機能だということ。

	// レンダーターゲットビューのハンドルを作成
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(rtvHeap->GetCPUDescriptorHandleForHeapStart(), frameIndex, rtvDescriptorSize);
	commandList->OMSetRenderTargets(1, &rtvHandle, false, nullptr);

	//バックバッファに描画(コマンドを記録)
	const FLOAT	clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };		// 青っぽい色
	commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);

	//三角形ポリゴン表示の設定
	if (pipelineState.Get() == nullptr)
	{
		return;
	}

	commandList->SetGraphicsRootSignature(rootSignature.Get());
	commandList->SetPipelineState(pipelineState.Get());
}

void Graphics::ScreenFlip()
{
	// バックバッファを表示
	auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(renderTargets[frameIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	commandList->ResourceBarrier(1, &barrier);

	// コマンドリストをクローズ
	commandList->Close();

	// コマンドリストを実行
	ID3D12CommandList* commandLists[] = { commandList.Get() };
	commandQueue->ExecuteCommandLists(_countof(commandLists), commandLists);

	// フレームを最終出力
	swapChain->Present(1, 0);

	// フレーム後処理
	WaitForPreviousFrame();
}

void Graphics::DrawTriangle(VECTOR lower_left, VECTOR upper_left, VECTOR lower_right)
{
	DirectX::XMFLOAT3 vertices[3] =
	{
		{lower_left.x, lower_left.y, lower_left.z},		//左下
		{upper_left.x, upper_left.y, upper_left.z},		//左上
		{lower_right.x, lower_right.y, lower_right.z},	//右下
	};

	D3D12_HEAP_PROPERTIES heapProp = {};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

	D3D12_RESOURCE_DESC resDesc = {};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeof(vertices);
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.Format = DXGI_FORMAT_UNKNOWN;
	resDesc.SampleDesc.Count = 1;
	resDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	ID3D12Resource* vertBuff = nullptr;
	if (FAILED(device->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff))))
	{
		return;
	}

	DirectX::XMFLOAT3* vertMap = nullptr;
	vertBuff->Map(0, nullptr, (void**)&vertMap);
	std::copy(std::begin(vertices), std::end(vertices), vertMap);
	vertBuff->Unmap(0, nullptr);

	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeof(vertices);
	vbView.StrideInBytes = sizeof(vertices[0]);

	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	commandList->IASetVertexBuffers(0, 1, &vbView);

	commandList->DrawInstanced(3, 1, 0, 0);
}

void Graphics::DrawRect(VECTOR lower_left, VECTOR upper_left, VECTOR upper_right, VECTOR lower_right)
{
	DirectX::XMFLOAT3 vertices[4] =
	{
		{lower_left.x, lower_left.y, lower_left.z},		//左下
		{upper_left.x, upper_left.y, upper_left.z},		//左上
		{lower_right.x, lower_right.y, lower_right.z},	//右下
		{upper_right.x, upper_right.y, upper_right.z},	//右上
	};

	D3D12_HEAP_PROPERTIES heapProp = {};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

	D3D12_RESOURCE_DESC resDesc = {};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeof(vertices);
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.Format = DXGI_FORMAT_UNKNOWN;
	resDesc.SampleDesc.Count = 1;
	resDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	ID3D12Resource* vertBuff = nullptr;
	if (FAILED(device->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff))))
	{
		return;
	}

	DirectX::XMFLOAT3* vertMap = nullptr;
	vertBuff->Map(0, nullptr, (void**)&vertMap);
	std::copy(std::begin(vertices), std::end(vertices), vertMap);
	vertBuff->Unmap(0, nullptr);

	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeof(vertices);
	vbView.StrideInBytes = sizeof(vertices[0]);

	unsigned short indices[] =
	{
		0, 1, 2,
		2, 1, 3,
	};

	resDesc.Width = sizeof(indices);

	ID3D12Resource* idxBuff = nullptr;

	//インデックスバッファーの作成
	device->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&idxBuff));

	//バッファーにインデックスデータをコピー
	unsigned short* mappedIdx = nullptr;
	idxBuff->Map(0, nullptr, (void**)&mappedIdx);
	std::copy(std::begin(indices), std::end(indices), mappedIdx);
	idxBuff->Unmap(0, nullptr);

	//インデックスバッファービューの作成
	D3D12_INDEX_BUFFER_VIEW ibView = {};
	ibView.BufferLocation = idxBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeof(indices);

	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	commandList->IASetVertexBuffers(0, 1, &vbView);
	commandList->IASetIndexBuffer(&ibView);
	commandList->DrawIndexedInstanced(6, 1, 0, 0, 0);
}

void Graphics::WaitForPreviousFrame()
{
	const UINT64 tmpFence = fenceValue;
	commandQueue->Signal(fence.Get(), tmpFence);
	fenceValue++;

	// 前のフレームが終了するまで待機
	if (fence->GetCompletedValue() < tmpFence) {
		fence->SetEventOnCompletion(tmpFence, fenceEvent);
		WaitForSingleObject(fenceEvent, INFINITE);
	}

	// バックバッファのインデックスを格納
	frameIndex = swapChain->GetCurrentBackBufferIndex();
}


