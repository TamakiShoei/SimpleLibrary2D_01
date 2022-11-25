#include "BufferManager.h"
#include <random>

std::map<int, SpriteData> BufferManager::canvasData;

int BufferManager::CreateCanvas(DirectX::TexMetadata metadata, const DirectX::Image* img, ID3D12Device* device)
{
	srand((unsigned)time(NULL));
	SpriteData data;

	int handle = 5000;

	vertBuff.Create(device);
	data.indexBuff = indexBuff.Create(device);
	data.texBuff = texBuff.Create(device, metadata, img);
	data.constBuff = constBuff.Create(device);

	data.metadata = metadata;

	do
	{
		key = rand() % 100000;

	} while (canvasData.contains(key));

	canvasData.insert(std::make_pair(key, data));

	return key;
}

void BufferManager::Finalize()
{
	vertBuff.Finalize();
}

ID3D12Resource* BufferManager::GetVertexBuffer(int key, ID3D12Device* device)
{
	if (B_FAILED(canvasData.contains(key)))
	{
		return nullptr;
	}
	return vertBuff.Get(device);
}

ID3D12Resource* BufferManager::GetIndexBuffer(int key)
{
	if (B_FAILED(canvasData.contains(key)))
	{
		return nullptr;
	}
	return canvasData.at(key).indexBuff;
}

ID3D12Resource* BufferManager::GetTexBuffer(int key)
{
	if (B_FAILED(canvasData.contains(key)))
	{
		return nullptr;
	}
	return canvasData.at(key).texBuff;
}

ID3D12Resource* BufferManager::GetConstantBuffer(int key)
{
	if (B_FAILED(canvasData.contains(key)))
	{
		return nullptr;
	}
	return canvasData.at(key).constBuff;
}

DirectX::TexMetadata BufferManager::GetMetadata(int key)
{
	return canvasData.at(key).metadata;
}

void BufferManager::ResetUseCounter()
{
	vertBuff.ResetUseCounter();
}
