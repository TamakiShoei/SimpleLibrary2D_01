#include "ConstantBuffer.h"

ConstantBuffer::ConstantBuffer()
{
	buffer = nullptr;
}

ConstantBuffer::~ConstantBuffer()
{

}

bool ConstantBuffer::Create(ID3D12Device* device)
{
	D3D12_HEAP_PROPERTIES heapProp = {};
	heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

	D3D12_RESOURCE_DESC resDesc = {};
	resDesc = CD3DX12_RESOURCE_DESC::Buffer((sizeof(DirectX::XMMATRIX) + 0xff) & ~0xff);

	if (FAILED(device->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&buffer))))
	{
		return true;
	}
	return true;
}

ID3D12Resource* ConstantBuffer::Get()
{
	return buffer;
}