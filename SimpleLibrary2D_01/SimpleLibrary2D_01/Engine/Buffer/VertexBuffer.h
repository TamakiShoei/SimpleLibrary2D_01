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
	* @retval true ��������
	* @retval false �������s
	*/
	ID3D12Resource* Create(ID3D12Device* device);
};

#endif