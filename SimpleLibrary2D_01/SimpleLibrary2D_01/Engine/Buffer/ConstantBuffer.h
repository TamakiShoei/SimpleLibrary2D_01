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
	* @retval true ��������
	* @retval false �������s
	*/
	ID3D12Resource* Create(ID3D12Device* device);
};

#endif