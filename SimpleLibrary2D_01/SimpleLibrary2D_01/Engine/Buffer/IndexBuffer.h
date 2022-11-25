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
	* @retval true ���������@
	* @retval false �������s
	*/
	ID3D12Resource* Create(ID3D12Device* device);

	/**
	* @brief �C���f�b�N�X�o�b�t�@�̎擾�֐�
	* @retval �C���f�b�N�X�o�b�t�@�f�[�^
	*/
	ID3D12Resource* Get();

private:
	ID3D12Resource* buffer;
	bool isCreated;
};

#endif