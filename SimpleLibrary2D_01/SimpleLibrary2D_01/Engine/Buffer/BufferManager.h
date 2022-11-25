#ifndef BUFFER_MANAGER_H
#define BUFFER_MANAGER_H

#include <map>
#include <vector>
#include "../CommonDifinition.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "TexBuffer.h"
#include "ConstantBuffer.h"

struct SpriteData
{
	DirectX::TexMetadata metadata;
	ID3D12Resource* vertBuff;
	ID3D12Resource* indexBuff;
	ID3D12Resource* texBuff;
	ID3D12Resource* constBuff;
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
	* @brief �e�o�b�t�@�̉���֐�
	*/
	void Finalize();

	/**
	* @brief ���_�o�b�t�@�̎擾�֐�
	* @param[in] key �L�[�l
	* @param[in] device �f�o�C�X
	*/
	ID3D12Resource* GetVertexBuffer(int key, ID3D12Device* device);

	/**
	* @brief �C���f�b�N�X�o�b�t�@�̎擾�֐�
	* @param[in] key �L�[�l
	* @retval �C���f�b�N�X�o�b�t�@�f�[�^
	*/
	ID3D12Resource* GetIndexBuffer(int key);

	/**
	* @brief �C���f�b�N�X�o�b�t�@�̎擾�֐�
	* @param[in] key �L�[�l
	* @retval �e�N�X�`���o�b�t�@�f�[�^
	*/
	ID3D12Resource* GetTexBuffer(int key);

	/**
	* @brief �C���f�b�N�X�o�b�t�@�̎擾�֐�
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

	/**
	* @brief ���_�o�b�t�@�N���X��UseCounter�̃��Z�b�g�֐�
	*/
	void ResetUseCounter();

private:
	VertexBuffer vertBuff;
	IndexBuffer indexBuff;
	TexBuffer texBuff;
	ConstantBuffer constBuff;
	int key;
	static std::map<int, SpriteData> canvasData;

private:	//�C���X�^���X�̐���
	BufferManager() = default;
	~BufferManager() = default;

	BufferManager(const BufferManager&) = delete;
	BufferManager& operator = (const BufferManager&) = delete;
	BufferManager(BufferManager&&) = delete;
	BufferManager& operator = (BufferManager&&) = delete;
};

#endif