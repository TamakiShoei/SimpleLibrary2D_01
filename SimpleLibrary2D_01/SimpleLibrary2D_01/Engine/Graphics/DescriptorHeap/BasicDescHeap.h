#ifndef DESCRIPTOR_HEAP_H
#define	DESCRIPTOR_HEAP_H

#include <map>
#include "../../CommonDifinition.h"

class BasicDescHeap
{
public:
	BasicDescHeap();
	~BasicDescHeap();

	bool Initialize(ID3D12Device* device);
	void RegisterCBV(D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc, int key, ID3D12Device* device);
	void RegisterSRV(ID3D12Resource* buff, D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc, int key, ID3D12Device* device);
	D3D12_GPU_DESCRIPTOR_HANDLE GetCBVHandle(int key, ID3D12Device* device);
	D3D12_GPU_DESCRIPTOR_HANDLE GetSRVHandle(int key, ID3D12Device* device);
	ID3D12DescriptorHeap* Get();
	void ResetCounter();
	void Finalize();

private:
	ComPtr<ID3D12DescriptorHeap> instance;
	D3D12_CPU_DESCRIPTOR_HANDLE handle;
	std::map<int, int> cbvDictionaly;
	std::map<int, int> srvDictionaly;
	int registerCounter;
};

#endif