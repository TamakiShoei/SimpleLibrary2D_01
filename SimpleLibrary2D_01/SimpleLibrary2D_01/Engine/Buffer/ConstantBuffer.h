/**
* @file ConstantBuffer.h
* @brief �R���X�^���g�o�b�t�@�[�N���X�̐錾
*/

#ifndef CONSTANT_BUFFER
#define CONSTANT_BUFFER

#include <map>
#include "../CommonDifinition.h"

class ConstantBuffer
{
public:
	ConstantBuffer();
	~ConstantBuffer();

	/**
	* @brief �R���X�^���g�o�b�t�@�̍쐬
	* @param[in] �f�o�C�X
	* @retval �R���X�^���g�o�b�t�@�̃C���X�^���X
	*/
	ID3D12Resource* Create(ID3D12Device* device);
};

#endif