#include "BufferManager.h"
#include <random>

int BufferManager::CreateCanvas(DirectX::TexMetadata metadata, const DirectX::Image* img, ID3D12Device* device)
{
	srand((unsigned)time(NULL));
	CanvasData data;

	data.buffers.vertBuff = CreateVertexBuffer(device);
	data.buffers.indexBuff = CreateIndexBuffer(device);
	data.buffers.texBuff = CreateTexBuffer(device, metadata, img);
	data.buffers.constBuff = CreateConstantBuffer(device);

	data.texData.metadata = metadata;
	data.texData.img = img;

	do
	{
		key = rand() % 100000;

	} while (canvasData.contains(key));

	canvasData.insert(std::make_pair(key, data));

	return key;
}

ID3D12Resource* BufferManager::CreateVertexBuffer(ID3D12Device* device)
{
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

	ID3D12Resource* tmpBuff = nullptr;
	if (FAILED(device->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&tmpBuff))))
	{
		return nullptr;
	}

	return tmpBuff;
}

ID3D12Resource* BufferManager::CreateIndexBuffer(ID3D12Device* device)
{
	D3D12_HEAP_PROPERTIES heapProp = {};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

	D3D12_RESOURCE_DESC resDesc = {};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeof(unsigned short) * 6;
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.Format = DXGI_FORMAT_UNKNOWN;
	resDesc.SampleDesc.Count = 1;
	resDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;


	ID3D12Resource* tmpBuff = nullptr;

	//�C���f�b�N�X�o�b�t�@�[�̍쐬
	if (FAILED(device->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&tmpBuff))))
	{
		return nullptr;
	}
	return tmpBuff;
}

ID3D12Resource* BufferManager::CreateTexBuffer(ID3D12Device* device, DirectX::TexMetadata metadata, const DirectX::Image* img)
{
	//WriteToSubresource�œ]�����邽�߂̃q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES heapProp = {};
	heapProp.Type = D3D12_HEAP_TYPE_CUSTOM;		//Type�̓J�X�^��
	heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;		//�]����CPU������
	//�P��A�_�v�^�[�̂���0
	heapProp.CreationNodeMask = 0;
	heapProp.VisibleNodeMask = 0;

	D3D12_RESOURCE_DESC resDesc = {};
	resDesc.Format = metadata.format; //�摜�f�[�^�̃t�H�[�}�b�g
	resDesc.Width = metadata.width;
	resDesc.Height = metadata.height;
	resDesc.DepthOrArraySize = metadata.arraySize;	//2D�Ŕz��ł��Ȃ�����1
	resDesc.SampleDesc.Count = 1;	//�A���`�G�C���A�V���O���Ȃ�
	resDesc.SampleDesc.Quality = 0;	//�N�I���e�B�͍Œ�
	resDesc.MipLevels = metadata.mipLevels;	//�~�b�v�}�b�v���Ȃ��̂Ń~�b�v����1
	resDesc.Dimension = static_cast<D3D12_RESOURCE_DIMENSION>(metadata.dimension);	//2D�e�N�X�`���p
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;	//���C�A�E�g�͌��肵�Ȃ�
	resDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

	ID3D12Resource* tmpBuff = nullptr;

	if (FAILED(device->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		nullptr,
		IID_PPV_ARGS(&tmpBuff))))
	{
		return nullptr;
	}

	tmpBuff->WriteToSubresource(
		0,
		nullptr,
		img->pixels,		//�S�f�[�^�̃A�h���X
		img->rowPitch,		//1���C���T�C�Y
		img->slicePitch);	//�ꖇ�̃T�C�Y

	return tmpBuff;
}

ID3D12Resource* BufferManager::CreateConstantBuffer(ID3D12Device* device)
{
	D3D12_HEAP_PROPERTIES heapProp = {};
	heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

	D3D12_RESOURCE_DESC resDesc = {};
	resDesc = CD3DX12_RESOURCE_DESC::Buffer((sizeof(DirectX::XMMATRIX) + 0xff) & ~0xff);

	ID3D12Resource* tmpBuff;

	if (FAILED(device->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&tmpBuff))))
	{
		return nullptr;
	}
	return tmpBuff;
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
	return canvasData.at(key).texData.metadata;
}

const DirectX::Image* BufferManager::GetImageData(int key)
{
	if (B_FAILED(canvasData.contains(key)))
	{
		return nullptr;
	}
	return canvasData.at(key).texData.img;
}
