#include "BufferManager.h"
#include <random>

std::map<int, CanvasData> BufferManager::canvasData;

int BufferManager::CreateCanvas(DirectX::TexMetadata metadata, const DirectX::Image* img, ID3D12Device* device)
{
	srand((unsigned)time(NULL));
	CanvasData data;

	if (vertBuff.Create(device) == false)
	{
		return -1;
	}
	if (indexBuff.Create(device) == false)
	{
		return -1;
	}
	if (texBuff.Create(device, metadata, img) == false)
	{
		return -1;
	}
	if (constBuff.Create(device) == false)
	{
		return -1;
	}
	
	data.buffers.vertBuff = vertBuff.Get();
	data.buffers.indexBuff = indexBuff.Get();
	data.buffers.texBuff = texBuff.Get();
	data.buffers.constBuff = constBuff.Get();

	data.metadata = metadata;

	do
	{
		key = rand() % 100000;

	} while (canvasData.contains(key));

	canvasData.insert(std::make_pair(key, data));

	return key;
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
	return canvasData.at(key).metadata;
}