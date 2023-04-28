/**
* @file IndexBuffer.h
* @brief �C���f�b�N�X�o�b�t�@�[�N���X�̐錾
*/
#ifndef INDEX_BUFFER
#define INDEX_BUFFER

#include <map>
#include "../CommonDifinition.h"

class IndexBuffer
{
public:
	IndexBuffer();
	~IndexBuffer();

	/**
	* @brief �C���f�b�N�X�o�b�t�@�̍쐬
	* @param[in] �f�o�C�X
	* @retval �C���f�b�N�X�o�b�t�@�̃C���X�^���X
	*/
	ID3D12Resource* Create(ID3D12Device* device);
};

#endif