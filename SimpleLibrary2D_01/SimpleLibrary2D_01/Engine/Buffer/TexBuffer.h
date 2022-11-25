#ifndef TEX_BUFFER
#define TEX_BUFFER

#include <map>
#include "../CommonDifinition.h"

class TexBuffer
{
public:
	TexBuffer();
	~TexBuffer();

	/**
	* @brief �e�N�X�`���o�b�t�@�̍쐬
	* @param[in] device �f�o�C�X
	* @param[in] metadata ���^�f�[�^
	* @param[in] img �摜���f�[�^
	* @retval true ��������
	* @retval false �������s
	*/
	ID3D12Resource* Create(ID3D12Device* device, DirectX::TexMetadata metadata, const DirectX::Image* img);

	/**
	* @brief �e�N�X�`���[�o�b�t�@�̎擾�֐�
	* @retval �e�N�X�`���[�o�b�t�@�f�[�^
	*/
	ID3D12Resource* Get();

private:
	ID3D12Resource* buffer;
};

#endif