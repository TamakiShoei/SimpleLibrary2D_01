/**
* @file VertexBuffer.h
* @brief ���_�o�b�t�@�N���X�̐錾
*/
#ifndef VERTEX_BUFFER
#define VERTEX_BUFFER

#include <map>
#include "../CommonDifinition.h"

class VertexBuffer
{
public:
	VertexBuffer();
	~VertexBuffer();

	/**
	* @brief ���_�o�b�t�@�̍쐬
	* @param[in] �f�o�C�X
	* @retval ���_�o�b�t�@�̃C���X�^���X
	*/
	ID3D12Resource* Create(ID3D12Device* device);
};

#endif