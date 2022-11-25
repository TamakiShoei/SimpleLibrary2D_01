#include "TexBuffer.h"

TexBuffer::TexBuffer()
{
	buffer = nullptr;
}

TexBuffer::~TexBuffer()
{

}

ID3D12Resource* TexBuffer::Create(ID3D12Device* device, DirectX::TexMetadata metadata, const DirectX::Image* img)
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

	if (FAILED(device->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		nullptr,
		IID_PPV_ARGS(&buffer))))
	{
		return buffer;
	}

	buffer->WriteToSubresource(
		0,
		nullptr,
		img->pixels,		//�S�f�[�^�̃A�h���X
		img->rowPitch,		//1���C���T�C�Y
		img->slicePitch);	//�ꖇ�̃T�C�Y

	return buffer;
}

ID3D12Resource* TexBuffer::Get()
{
	return buffer;
}