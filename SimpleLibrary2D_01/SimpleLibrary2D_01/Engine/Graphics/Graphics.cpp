#include <locale>
#include <time.h>
#include <random>
#include "Graphics.h"

bool Graphics::Initialize()
{
	srand((unsigned)time(NULL));

	UINT dxgiFactoryFlags = 0;

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
		debugController->Release();
	}
#endif
	if (B_FAILED(factory.Initialize(dxgiFactoryFlags)))
	{
		return false;
	}
	if (B_FAILED(device.Initialize(factory.Get())))
	{
		return false;
	}
	if (B_FAILED(commandQueue.Initialize(device.Get())))
	{
		return false;
	}
	if (B_FAILED(swapChain.Initialize(factory.Get(), commandQueue.Get())))
	{
		return false;
	}
	if (B_FAILED(rootSignature.Initialize(device.Get())))
	{
		return false;
	}
	if (B_FAILED(pipeline.Initialize(device.Get(), rootSignature.Get())))
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

	heap.Initialize(device.Get());

	if (B_FAILED(commandAllocator.Initialize(device.Get())))
	{
		return false;
	}
	if (B_FAILED(commandList.Initialize(device.Get(), commandAllocator.Get())))
	{
		return false;
	}

	//ビューポートとシザーの設定
	SetDrawArea();

	commandList.Get()->Close();
	// コマンドリストは記録状態で作成されるが、今回は初期化内でそこに何も入れないのですぐに閉じる。

	if (B_FAILED(InitializeFence()))
	{
		return false;
	}

	return true;
}

bool Graphics::InitializeFence()
{
	// フェンスを作成
	if (FAILED(device.Get()->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(fence.GetAddressOf()))))
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
	if (FAILED(device.Get()->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(rtvHeap.GetAddressOf()))))
	{
		MessageBox(NULL, L"ディスクリプタヒープを作成できませんでした。", WINDOW_TITLE, MB_OK | MB_ICONERROR);
		return false;
	}

	//ディスクリプタのサイズを取得
	rtvDescriptorSize = device.Get()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	return true;
}

bool Graphics::CreateRenderTargetView()
{
	//フレームリソースのハンドルを取得
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(rtvHeap->GetCPUDescriptorHandleForHeapStart());

	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;	//ガンマ補正あり(SRGB)
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

	// フレームバッファとバックバッファのレンダーターゲットビューを作成
	for (UINT i = 0; i < frameCount; i++)
	{
		// RTVバッファを取得
		if (FAILED(swapChain.Get()->GetBuffer(i, IID_PPV_ARGS(renderTargets[i].GetAddressOf()))))
		{
			MessageBox(NULL, L"RTVバッファを取得できませんでした。", WINDOW_TITLE, MB_OK | MB_ICONERROR);
			return false;
		}

		// レンダーターゲットビューを作成
		device.Get()->CreateRenderTargetView(renderTargets[i].Get(), &rtvDesc, rtvHandle);

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

	viewport.Width = static_cast<float>(rect.right);
	viewport.Height = static_cast<float>(rect.bottom);
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MaxDepth = 1.0f;
	viewport.MinDepth = 0.0f;

	scissorRect.top = 0;
	scissorRect.left = 0;
	scissorRect.right = scissorRect.left + rect.right;
	scissorRect.bottom = scissorRect.top + rect.bottom;
}

void Graphics::ClearScreen()
{
	// コマンドアロケータをリセット
	commandAllocator.Get()->Reset();

	// コマンドリストをリセット
	commandList.Get()->Reset(commandAllocator.Get(), pipeline.Get());

	//ビューポートのセット
	commandList.Get()->RSSetViewports(1, &viewport);
	//シザーのセット
	commandList.Get()->RSSetScissorRects(1, &scissorRect);

	BufferManager::Instance()->ResetUseCounter();

	heap.ResetCounter();

	// バックバッファのインデックスを格納
	frameIndex = swapChain.Get()->GetCurrentBackBufferIndex();

	// バックバッファをレンダーターゲットとして使用
	auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(renderTargets[frameIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	commandList.Get()->ResourceBarrier(1, &barrier);
	// リソースバリアとは、GPU側で扱うリソースの状況を同期させる機能。
	// マルチスレッドを前提とした動きなので、GPU側の動作も複数のアクセスが同時に行われることを想定した機能だということ。

	// レンダーターゲットビューのハンドルを作成
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(rtvHeap->GetCPUDescriptorHandleForHeapStart(), frameIndex, rtvDescriptorSize);
	commandList.Get()->OMSetRenderTargets(1, &rtvHandle, false, nullptr);

	//バックバッファに描画(コマンドを記録)
	const FLOAT	clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };		// 青っぽい色
	commandList.Get()->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);

	if (pipeline.Get() == nullptr)
	{
		return;
	}

	commandList.Get()->SetGraphicsRootSignature(rootSignature.Get());
	commandList.Get()->SetPipelineState(pipeline.Get());
}

void Graphics::ScreenFlip()
{
	// バックバッファを表示
	auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(renderTargets[frameIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	commandList.Get()->ResourceBarrier(1, &barrier);

	// コマンドリストをクローズ
	commandList.Get()->Close();

	// コマンドリストを実行
	ID3D12CommandList* commandLists[] = { commandList.Get() };
	commandQueue.Get()->ExecuteCommandLists(_countof(commandLists), commandLists);

	// フレームを最終出力
	swapChain.Get()->Present(1, 0);

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
	if (FAILED(device.Get()->CreateCommittedResource(
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
	//vertBuff->Map(0, nullptr, (void**)&vertMap);
	std::copy(std::begin(vertices), std::end(vertices), vertMap);
	vertBuff->Unmap(0, nullptr);

	D3D12_VERTEX_BUFFER_VIEW vbView;
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeof(vertices);
	vbView.StrideInBytes = sizeof(vertices[0]);

	commandList.Get()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	commandList.Get()->IASetVertexBuffers(0, 1, &vbView);

	commandList.Get()->DrawInstanced(3, 1, 0, 0);
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
	resDesc.Width = sizeof(Vertex) * 4;
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.Format = DXGI_FORMAT_UNKNOWN;
	resDesc.SampleDesc.Count = 1;
	resDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	ID3D12Resource* vertBuff = nullptr;
	if (FAILED(device.Get()->CreateCommittedResource(
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

	D3D12_VERTEX_BUFFER_VIEW vbView;
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
	device.Get()->CreateCommittedResource(
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

	commandList.Get()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	commandList.Get()->IASetVertexBuffers(0, 1, &vbView);
	commandList.Get()->IASetIndexBuffer(&ibView);
	commandList.Get()->DrawIndexedInstanced(6, 1, 0, 0, 0);
}

int Graphics::LoadTexture(const char* file_path)
{
	//WICテクスチャのロード
	DirectX::TexMetadata metadata = {};
	DirectX::ScratchImage scratchImg = {};

	//const char* から const wchar_t*に変換
	const int MaxChar = 150;
	wchar_t wFilePath[MaxChar];
	size_t ret;
	setlocale(LC_CTYPE, "jpn");
	mbstowcs_s(&ret, wFilePath, MaxChar, file_path, _TRUNCATE);

	auto result = DirectX::LoadFromWICFile(
		wFilePath,
		DirectX::WIC_FLAGS_NONE,
		&metadata,
		scratchImg);

	//生データの抽出
	auto img = scratchImg.GetImage(0, 0, 0);

	int key = BufferManager::Instance()->CreateSprite(metadata, img, device.Get());

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	heap.RegisterSRV(BufferManager::Instance()->GetTexBuffer(key), srvDesc, key, device.Get());

	///////////////

	ID3D12Resource* constBuff = BufferManager::Instance()->GetConstantBuffer(key);
	DirectX::XMMATRIX matrix = DirectX::XMMatrixIdentity();

	//座標返還
	matrix.r[0].m128_f32[0] = 2.0f / viewport.Width;
	matrix.r[1].m128_f32[1] = -2.0f / viewport.Height;

	matrix.r[3].m128_f32[0] = -1.0f;
	matrix.r[3].m128_f32[1] = 1.0f;

	DirectX::XMMATRIX* mapMatrix;	//マップ先を示すポインタ
	constBuff->Map(0, nullptr, (void**)&mapMatrix);	//マップ
	*mapMatrix = matrix;
	//コンスタントバッファービュー設定
	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
	cbvDesc.BufferLocation = constBuff->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = constBuff->GetDesc().Width;
	heap.RegisterCBV(cbvDesc, key, device.Get());

	return key;
}

void Graphics::DrawTexture(float pos_x, float pos_y, int key)
{
	DirectX::TexMetadata metadata = BufferManager::Instance()->GetMetadata(key);

	Vertex vertices[4] =
	{
		{{pos_x, pos_y + metadata.height, 0.0f}, {0.0f, 1.0f}},	//左下
		{{pos_x, pos_y, 0.0f}, {0.0f, 0.0f}},		//左上
		{{pos_x + metadata.width, pos_y + metadata.height, 0.0f}, {1.0f, 1.0f} },	//右下
		{{pos_x + metadata.width, pos_y, 0.0f}, {1.0f, 0.0f} },	//右上
	};

	ID3D12Resource* vertBuff = nullptr;
	vertBuff = BufferManager::Instance()->GetVertexBuffer(key, device.Get());

	Vertex* vertMap = nullptr;
	vertBuff->Map(0, nullptr, (void**)&vertMap);
	std::copy(std::begin(vertices), std::end(vertices), vertMap);
	vertBuff->Unmap(0, nullptr);

	D3D12_VERTEX_BUFFER_VIEW vbView;
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeof(vertices);
	vbView.StrideInBytes = sizeof(vertices[0]);

	unsigned short indices[] =
	{
		0, 1, 2,
		2, 1, 3,
	};

	ID3D12Resource* idxBuff = BufferManager::Instance()->GetIndexBuffer(key);

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

	//DescriptorHrap作成
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;		//シェーダーから見えるように
	descHeapDesc.NodeMask = 0;
	descHeapDesc.NumDescriptors = 2;	//SRVとCBV
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	commandList.Get()->SetGraphicsRootSignature(rootSignature.Get());

	auto tmpHeap = heap.Get();
	commandList.Get()->SetDescriptorHeaps(1, &tmpHeap);

	//ルートパラメーターとディスクリプタヒープのバインド

	commandList.Get()->SetGraphicsRootDescriptorTable(0, heap.GetSRVHandle(key, device.Get()));
	//auto heapHandle = basicDescHeap->GetGPUDescriptorHandleForHeapStart();
	//heapHandle.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	commandList.Get()->SetGraphicsRootDescriptorTable(1, heap.GetCBVHandle(key, device.Get()));

	commandList.Get()->IASetVertexBuffers(0, 1, &vbView);
	commandList.Get()->IASetIndexBuffer(&ibView);
	commandList.Get()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	commandList.Get()->DrawIndexedInstanced(6, 1, 0, 0, 0);
}

void Graphics::WaitForPreviousFrame()
{
	const UINT64 tmpFence = fenceValue;
	commandQueue.Get()->Signal(fence.Get(), tmpFence);
	fenceValue++;

	// 前のフレームが終了するまで待機
	if (fence->GetCompletedValue() < tmpFence) {
		fence->SetEventOnCompletion(tmpFence, fenceEvent);
		WaitForSingleObject(fenceEvent, INFINITE);
	}

	// バックバッファのインデックスを格納
	frameIndex = swapChain.Get()->GetCurrentBackBufferIndex();
}

void Graphics::Finalize()
{
	fence->Release();
	commandList.Finalize();
	commandAllocator.Finalize();
	renderTargets[1].ReleaseAndGetAddressOf();
	renderTargets[0].ReleaseAndGetAddressOf();
	rtvHeap->Release();
	rootSignature.Finalize();
	pipeline.Finalize();
	swapChain.Finalize();
	commandQueue.Finalize();
	factory.Finalize();
	heap.Finalize();
	device.Finalize();

	////リークを確かめるためのデバッグ↓↓↓
	//ComPtr<ID3D12DebugDevice> debugDevice;
	//// 作成
	//HRESULT result = device->QueryInterface(debugDevice.GetAddressOf());
	//if (FAILED(result))
	//{
	//	return;
	//}

	//// 詳細表示
	//result = debugDevice->ReportLiveDeviceObjects(D3D12_RLDO_DETAIL);
	//debugDevice->Release();
}


