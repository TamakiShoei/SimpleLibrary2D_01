#include "TexBuffer.h"

TexBuffer::TexBuffer()
{
	buffer = nullptr;
}

TexBuffer::~TexBuffer()
{

}

ID3D12Resource* TexBuffer::Create(ID3D12Device* device, DirectX::TexMetadata metadata, const DirectX::Image* img)
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

	if (FAILED(device->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		nullptr,
		IID_PPV_ARGS(&buffer))))
	{
		return buffer;
	}

	buffer->WriteToSubresource(
		0,
		nullptr,
		img->pixels,		//全データのアドレス
		img->rowPitch,		//1ラインサイズ
		img->slicePitch);	//一枚のサイズ

	return buffer;
}

ID3D12Resource* TexBuffer::Get()
{
	return buffer;
}