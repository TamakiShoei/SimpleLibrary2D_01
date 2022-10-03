#ifndef BUFFER_MANAGER_H
#define BUFFER_MANAGER_H

#include <map>
#include "../CommonDifinition.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "TexBuffer.h"
#include "ConstantBuffer.h"

struct Buffers
{
	ID3D12Resource*	indexBuff;
	ID3D12Resource*	texBuff;
	ID3D12Resource*	constBuff;
	ID3D12Resource*	vertBuff;
};

struct TextureData
{
	DirectX::TexMetadata metadata;
	const DirectX::Image* img;
};

struct CanvasData
{
	TextureData texData;
	Buffers buffers;
};

class BufferManager
{
public:
	static BufferManager* Instance()
	{
		static BufferManager instance;
		return &instance;
	}

	int CreateCanvas(DirectX::TexMetadata metadata, const DirectX::Image* img, ID3D12Device* device);

	void Updata(int key);

	ID3D12Resource* GetVertexBuffer(int key);
	ID3D12Resource* GetIndexBuffer(int key);
	ID3D12Resource* GetTexBuffer(int key);
	ID3D12Resource* GetConstantBuffer(int key);
	DirectX::TexMetadata GetMetadata(int key);
	const DirectX::Image* GetImageData(int key);

private:
	ID3D12Resource* CreateVertexBuffer(ID3D12Device* device);
	ID3D12Resource* CreateIndexBuffer(ID3D12Device* device);
	ID3D12Resource* CreateTexBuffer(ID3D12Device* device, DirectX::TexMetadata metadata, const DirectX::Image* img);
	ID3D12Resource* CreateConstantBuffer(ID3D12Device* device);

private:
	int key;
	static std::map<int, CanvasData> canvasData;

private:	//インスタンスの制限
	BufferManager() = default;
	~BufferManager() = default;

	BufferManager(const BufferManager&) = delete;
	BufferManager& operator = (const BufferManager&) = delete;
	BufferManager(BufferManager&&) = delete;
	BufferManager& operator = (BufferManager&&) = delete;
};

#endif