#include "BufferManager.h"

bool BufferManager::CreateCanvas(DirectX::TexMetadata metadata, const DirectX::Image* img, ID3D12Device* device)
{
	CanvasData data;

	data.buffers.vertBuff = CreateVertexBuffer(device);
	data.buffers.indexBuff = CreateIndexBuffer(device);
	data.buffers.texBuff = CreateTexBuffer(device, metadata);
	data.buffers.constBuff = CreateConstantBuffer(device);

	int key = 123456;
	canvasData.insert(std::make_pair(key, data));

	return true;
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

ID3D12Resource* BufferManager::CreateTexBuffer(ID3D12Device* device, DirectX::TexMetadata metadata)
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

ID3D12Resource* BufferManager::GetVertexBuffer()
{
	return canvasData.at(123456).buffers.vertBuff;
}

ID3D12Resource* BufferManager::GetIndexBuffer()
{
	return canvasData.at(123456).buffers.indexBuff;
}

ID3D12Resource* BufferManager::GetTexBuffer()
{
	return canvasData.at(123456).buffers.texBuff;
}

ID3D12Resource* BufferManager::GetConstantBuffer()
{
	return canvasData.at(123456).buffers.constBuff;
}