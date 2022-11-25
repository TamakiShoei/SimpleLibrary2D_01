#include "VertexBuffer.h"

VertexBuffer::VertexBuffer()
{
	buffers.clear();
	useCounter = 0;
}

VertexBuffer::~VertexBuffer()
{

}

void VertexBuffer::Create(ID3D12Device* device)
{
	ID3D12Resource* tmp;

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

	if (FAILED(device->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&tmp))))
	{
		return;
	}

	buffers.push_back(tmp);

	return;
}

ID3D12Resource* VertexBuffer::Get(ID3D12Device* device)
{
	if (useCounter >= buffers.size())
	{
		Create(device);
	}
	ID3D12Resource* tmp = buffers.at(useCounter);
	useCounter++;
	return tmp;
}

void VertexBuffer::Finalize()
{
	for (auto& itr : buffers)
	{
		itr->Release();
	}
	buffers.clear();
}

void VertexBuffer::ResetUseCounter()
{
	useCounter = 0;
}