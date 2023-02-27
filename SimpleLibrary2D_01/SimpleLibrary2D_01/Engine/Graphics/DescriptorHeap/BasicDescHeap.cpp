#include "BasicDescHeap.h"

BasicDescHeap::BasicDescHeap() : registerCounter(1)
{

}

BasicDescHeap::~BasicDescHeap()
{

}

bool BasicDescHeap::Initialize(ID3D12Device* device)
{
	//BasicDescriptorHrap作成
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;		//シェーダーから見えるように
	descHeapDesc.NodeMask = 0;
	descHeapDesc.NumDescriptors = 1024;
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	if (FAILED(device->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&instance))))
	{
		return false;
	}

	D3D12_CPU_DESCRIPTOR_HANDLE handle = instance->GetCPUDescriptorHandleForHeapStart();

	return true;
}

void BasicDescHeap::RegisterCBV(D3D12_CONSTANT_BUFFER_VIEW_DESC cbv_desc, int key, ID3D12Device* device)
{
	//DescriptorHeapの先頭アドレスを取得
	D3D12_CPU_DESCRIPTOR_HANDLE handle = instance->GetCPUDescriptorHandleForHeapStart();

	//先頭からインクリメントサイズ * カウンター分動かした位置のアドレスを取得
	handle.ptr += 
		device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * registerCounter;

	//上で取得したアドレス位置に定数バッファを作成して登録
	device->CreateConstantBufferView(&cbv_desc, handle);

	//Dictionalyに画像ハンドルとカウンター番号がペアになったデータを挿入
	cbvDictionaly.insert(std::make_pair(key, registerCounter));

	//カウンターをインクリメント
	registerCounter++;
}

void BasicDescHeap::RegisterSRV(ID3D12Resource* buff, D3D12_SHADER_RESOURCE_VIEW_DESC srv_desc, int key, ID3D12Device* device)
{
	//DescriptorHeapの先頭アドレスを取得
	D3D12_CPU_DESCRIPTOR_HANDLE handle = instance->GetCPUDescriptorHandleForHeapStart();

	//先頭からインクリメントサイズ * カウンター分動かした位置のアドレスを取得
	handle.ptr += 
		device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * registerCounter;

	//上で取得したアドレス位置に定数バッファを作成して登録
	device->CreateShaderResourceView(buff, &srv_desc, handle);

	//Dictionalyに画像ハンドルとカウンター番号がペアになったデータを挿入
	srvDictionaly.insert(std::make_pair(key, registerCounter));
	
	//カウンターをインクリメント
	registerCounter++;
}

D3D12_GPU_DESCRIPTOR_HANDLE BasicDescHeap::GetSRVHandle(int key, ID3D12Device* device)
{
	D3D12_GPU_DESCRIPTOR_HANDLE handle = instance->GetGPUDescriptorHandleForHeapStart();
	handle.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * srvDictionaly.at(key);

	return handle;
}

D3D12_GPU_DESCRIPTOR_HANDLE BasicDescHeap::GetCBVHandle(int key, ID3D12Device* device)
{
	D3D12_GPU_DESCRIPTOR_HANDLE handle = instance->GetGPUDescriptorHandleForHeapStart();
	handle.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * cbvDictionaly.at(key);

	return handle;
}

ID3D12DescriptorHeap* BasicDescHeap::Get()
{
	return instance.Get();
}

void BasicDescHeap::ResetCounter()
{
	registerCounter = 1;
}

void BasicDescHeap::Finalize()
{
	srvDictionaly.clear();
	cbvDictionaly.clear();

	instance.ReleaseAndGetAddressOf();
}