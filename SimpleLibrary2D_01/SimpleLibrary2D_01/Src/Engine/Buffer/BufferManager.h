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

	/**
	* @brief �L�����o�X�̍쐬�֐�
	* @param[in] metadata �摜�̃��^�f�[�^
	* @param[in] img �摜�̐��f�[�^
	* @param[in] device �f�o�C�X
	*/
	int CreateCanvas(DirectX::TexMetadata metadata, const DirectX::Image* img, ID3D12Device* device);

	/**
	* @brief ���_�o�b�t�@�̎擾�֐�
	* @param[in] key �L�[�l
	* @retval ���_�o�b�t�@�f�[�^
	*/
	ID3D12Resource* GetVertexBuffer(int key);

	/**
	* @brief �C���f�b�N�X�o�b�t�@�̎擾�֐�
	* @param[in] key �L�[�l
	* @retval �C���f�b�N�X�o�b�t�@�f�[�^
	*/
	ID3D12Resource* GetIndexBuffer(int key);

	/**
	* @brief �e�N�X�`���[�o�b�t�@�̎擾�֐�
	* @param[in] key �L�[�l
	* @retval �e�N�X�`���[�o�b�t�@�f�[�^
	*/
	ID3D12Resource* GetTexBuffer(int key);

	/**
	* @brief �R���X�^���g�o�b�t�@�̎擾�֐�
	* @param[in] key �L�[�l
	* @retval �R���X�^���g�o�b�t�@�f�[�^
	*/
	ID3D12Resource* GetConstantBuffer(int key);

	/**
	* @brief ���^�f�[�^�̎擾�֐�
	* @param[in] key �L�[�l
	* @retval ���^�f�[�^
	*/
	DirectX::TexMetadata GetMetadata(int key);

private:

	/**
	* @brief ���_�o�b�t�@�̍쐬
	* @param[in] �f�o�C�X
	* @retval ���_�o�b�t�@�f�[�^
	*/
	ID3D12Resource* CreateVertexBuffer(ID3D12Device* device);

	/**
	* @brief �C���f�b�N�X�o�b�t�@�̍쐬
	* @param[in] �f�o�C�X
	* @retval �C���f�b�N�X�o�b�t�@�f�[�^
	*/
	ID3D12Resource* CreateIndexBuffer(ID3D12Device* device);

	/**
	* @brief �e�N�X�`���o�b�t�@�̍쐬
	* @param[in] �f�o�C�X
	* @retval �e�N�X�`���o�b�t�@�f�[�^
	*/
	ID3D12Resource* CreateTexBuffer(ID3D12Device* device, DirectX::TexMetadata metadata, const DirectX::Image* img);

	/**
	* @brief �R���X�^���g�o�b�t�@�̍쐬
	* @param[in] �f�o�C�X
	* @retval �R���X�^���g�o�b�t�@�f�[�^
	*/
	ID3D12Resource* CreateConstantBuffer(ID3D12Device* device);

private:
	int key;
	static std::map<int, CanvasData> canvasData;

private:	//�C���X�^���X�̐���
	BufferManager() = default;
	~BufferManager() = default;

	BufferManager(const BufferManager&) = delete;
	BufferManager& operator = (const BufferManager&) = delete;
	BufferManager(BufferManager&&) = delete;
	BufferManager& operator = (BufferManager&&) = delete;
};

#endif