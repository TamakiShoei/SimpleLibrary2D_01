#include "DescriptorHeap.h"

BasicDescHeap::BasicDescHeap() : registerCounter(1)
{

}

BasicDescHeap::~BasicDescHeap()
{

}

void BasicDescHeap::Initialize(ID3D12Device* device)
{
	//BasicDescriptorHrap作成
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;		//シェーダーから見えるように
	descHeapDesc.NodeMask = 0;
	descHeapDesc.NumDescriptors = 1024;	//SRVとCBV
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	device->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&heap));

	D3D12_CPU_DESCRIPTOR_HANDLE handle = heap->GetCPUDescriptorHandleForHeapStart();
}

void BasicDescHeap::RegisterCBV(D3D12_CONSTANT_BUFFER_VIEW_DESC cbv_desc, int key, ID3D12Device* device)
{
	D3D12_CPU_DESCRIPTOR_HANDLE handle = heap->GetCPUDescriptorHandleForHeapStart();

	handle.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * registerCounter;

	device->CreateConstantBufferView(&cbv_desc, handle);

	cbvDictionaly.insert(std::make_pair(key, registerCounter));

	registerCounter++;
}

void BasicDescHeap::RegisterSRV(ID3D12Resource* buff, D3D12_SHADER_RESOURCE_VIEW_DESC srv_desc, int key, ID3D12Device* device)
{
	D3D12_CPU_DESCRIPTOR_HANDLE handle = heap->GetCPUDescriptorHandleForHeapStart();

	handle.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * registerCounter;

	device->CreateShaderResourceView(buff, &srv_desc, handle);

	srvDictionaly.insert(std::make_pair(key, registerCounter));

	registerCounter++;
}

D3D12_GPU_DESCRIPTOR_HANDLE BasicDescHeap::GetSRVHandle(int key, ID3D12Device* device)
{
	D3D12_GPU_DESCRIPTOR_HANDLE handle = heap->GetGPUDescriptorHandleForHeapStart();
	handle.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * srvDictionaly.at(key);

	return handle;
}

D3D12_GPU_DESCRIPTOR_HANDLE BasicDescHeap::GetCBVHandle(int key, ID3D12Device* device)
{
	D3D12_GPU_DESCRIPTOR_HANDLE handle = heap->GetGPUDescriptorHandleForHeapStart();
	handle.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * cbvDictionaly.at(key);

	return handle;
}

ID3D12DescriptorHeap* BasicDescHeap::Get()
{
	return heap.Get();
}

void BasicDescHeap::ResetCounter()
{
	registerCounter = 1;
}

void BasicDescHeap::Finalize()
{
	srvDictionaly.clear();
	cbvDictionaly.clear();

	heap.ReleaseAndGetAddressOf();
}