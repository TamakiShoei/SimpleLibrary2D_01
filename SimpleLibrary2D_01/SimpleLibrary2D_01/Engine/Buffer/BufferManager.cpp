#include "BufferManager.h"
#include <random>

int BufferManager::CreateSprite(DirectX::TexMetadata metadata, const DirectX::Image* img, ID3D12Device* device)
{
	srand((unsigned)time(NULL));
	SpriteData data;

	data.vertBuff.push_back(vertBuff.Create(device));
	data.indexBuff = indexBuff.Create(device);
	data.texBuff = texBuff.Create(device, metadata, img);
	data.constBuff = constBuff.Create(device);

	data.metadata = metadata;

	do
	{
		key = rand() % 100000;

	} while (spriteData.contains(key));

	spriteData.insert(std::make_pair(key, data));
	useCounter.insert(std::make_pair(key, 0));

	return key;
}

ID3D12Resource* BufferManager::GetVertexBuffer(int key, ID3D12Device* device)
{
	if (B_FAILED(spriteData.contains(key)))
	{
		return nullptr;
	}
	if (spriteData.at(key).vertBuff.size() <= useCounter.at(key))
	{
		spriteData.at(key).vertBuff.push_back(vertBuff.Create(device));
	}
	useCounter.at(key) += 1;
	return spriteData.at(key).vertBuff.at(useCounter.at(key) - 1);
}

ID3D12Resource* BufferManager::GetIndexBuffer(int key)
{
	if (B_FAILED(spriteData.contains(key)))
	{
		return nullptr;
	}
	return spriteData.at(key).indexBuff;
}

ID3D12Resource* BufferManager::GetTexBuffer(int key)
{
	if (B_FAILED(spriteData.contains(key)))
	{
		return nullptr;
	}
	return spriteData.at(key).texBuff;
}

ID3D12Resource* BufferManager::GetConstantBuffer(int key)
{
	if (B_FAILED(spriteData.contains(key)))
	{
		return nullptr;
	}
	return spriteData.at(key).constBuff;
}

DirectX::TexMetadata BufferManager::GetMetadata(int key)
{
	return spriteData.at(key).metadata;
}

void BufferManager::ResetUseCounter()
{
	for (auto& itr : useCounter)
	{
		itr.second = 0;
	}
}

void BufferManager::Finalize()
{
	for (auto& itr : spriteData)
	{
		for (auto& vert : itr.second.vertBuff)
		{
			vert->Release();
			vert = nullptr;
		}
		itr.second.vertBuff.clear();
		itr.second.indexBuff->Release();
		itr.second.indexBuff = nullptr;
		itr.second.texBuff->Release();
		itr.second.texBuff = nullptr;
		itr.second.constBuff->Release();
		itr.second.constBuff = nullptr;
	}
}
