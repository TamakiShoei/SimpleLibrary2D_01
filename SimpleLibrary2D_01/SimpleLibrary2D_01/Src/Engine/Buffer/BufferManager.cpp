#include "BufferManager.h"
#include <random>

int BufferManager::CreateCanvas(DirectX::TexMetadata metadata, const DirectX::Image* img, ID3D12Device* device)
{
	srand((unsigned)time(NULL));
	CanvasData data;

	data.buffers.vertBuff = CreateVertexBuffer(device);
	data.buffers.indexBuff = CreateIndexBuffer(device);
	data.buffers.texBuff = CreateTexBuffer(device, metadata, img);
	data.buffers.constBuff = CreateConstantBuffer(device);

	data.texData.metadata = metadata;
	data.texData.img = img;

	do
	{
		key = rand() % 100000;

	} while (canvasData.contains(key));

	canvasData.insert(std::make_pair(key, data));

	return key;
}

ID3D12Resource* BufferManager::CreateVertexBuffer(ID3D12Device* device)
{
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

	ID3D12Resource* tmpBuff = nullptr;
	if (FAILED(device->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&tmpBuff))))
	{
		return nullptr;
	}

	return tmpBuff;
}

ID3D12Resource* BufferManager::CreateIndexBuffer(ID3D12Device* device)
{
	D3D12_HEAP_PROPERTIES heapProp = {};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

	D3D12_RESOURCE_DESC resDesc = {};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeof(unsigned short) * 6;
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.Format = DXGI_FORMAT_UNKNOWN;
	resDesc.SampleDesc.Count = 1;
	resDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;


	ID3D12Resource* tmpBuff = nullptr;

	//インデックスバッファーの作成
	if (FAILED(device->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&tmpBuff))))
	{
		return nullptr;
	}
	return tmpBuff;
}

ID3D12Resource* BufferManager::CreateTexBuffer(ID3D12Device* device, DirectX::TexMetadata metadata, const DirectX::Image* img)
{
	//WriteToSubresourceで転送するためのヒープ設定
	D3D12_HEAP_PROPERTIES heapProp = {};
	heapProp.Type = D3D12_HEAP_TYPE_CUSTOM;		//Typeはカスタム
	heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;		//転送はCPU側から
	//単一アダプターのため0
	heapProp.CreationNodeMask = 0;
	heapProp.VisibleNodeMask = 0;

	D3D12_RESOURCE_DESC resDesc = {};
	resDesc.Format = metadata.format; //画像データのフォーマット
	resDesc.Width = metadata.width;
	resDesc.Height = metadata.height;
	resDesc.DepthOrArraySize = metadata.arraySize;	//2Dで配列でもないため1
	resDesc.SampleDesc.Count = 1;	//アンチエイリアシングしない
	resDesc.SampleDesc.Quality = 0;	//クオリティは最低
	resDesc.MipLevels = metadata.mipLevels;	//ミップマップしないのでミップ数は1
	resDesc.Dimension = static_cast<D3D12_RESOURCE_DIMENSION>(metadata.dimension);	//2Dテクスチャ用
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;	//レイアウトは決定しない
	resDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

	ID3D12Resource* tmpBuff = nullptr;

	if (FAILED(device->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		nullptr,
		IID_PPV_ARGS(&tmpBuff))))
	{
		return nullptr;
	}

	tmpBuff->WriteToSubresource(
		0,
		nullptr,
		img->pixels,		//全データのアドレス
		img->rowPitch,		//1ラインサイズ
		img->slicePitch);	//一枚のサイズ

	return tmpBuff;
}

ID3D12Resource* BufferManager::CreateConstantBuffer(ID3D12Device* device)
{
	D3D12_HEAP_PROPERTIES heapProp = {};
	heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

	D3D12_RESOURCE_DESC resDesc = {};
	resDesc = CD3DX12_RESOURCE_DESC::Buffer((sizeof(DirectX::XMMATRIX) + 0xff) & ~0xff);

	ID3D12Resource* tmpBuff;

	if (FAILED(device->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&tmpBuff))))
	{
		return nullptr;
	}
	return tmpBuff;
}

ID3D12Resource* BufferManager::GetVertexBuffer(int key)
{
	if (B_FAILED(canvasData.contains(key)))
	{
		return nullptr;
	}
	return canvasData.at(key).buffers.vertBuff;
}

ID3D12Resource* BufferManager::GetIndexBuffer(int key)
{
	if (B_FAILED(canvasData.contains(key)))
	{
		return nullptr;
	}
	return canvasData.at(key).buffers.indexBuff;
}

ID3D12Resource* BufferManager::GetTexBuffer(int key)
{
	if (B_FAILED(canvasData.contains(key)))
	{
		return nullptr;
	}
	return canvasData.at(key).buffers.texBuff;
}

ID3D12Resource* BufferManager::GetConstantBuffer(int key)
{
	if (B_FAILED(canvasData.contains(key)))
	{
		return nullptr;
	}
	return canvasData.at(key).buffers.constBuff;
}

DirectX::TexMetadata BufferManager::GetMetadata(int key)
{
	return canvasData.at(key).texData.metadata;
}

const DirectX::Image* BufferManager::GetImageData(int key)
{
	if (B_FAILED(canvasData.contains(key)))
	{
		return nullptr;
	}
	return canvasData.at(key).texData.img;
}
