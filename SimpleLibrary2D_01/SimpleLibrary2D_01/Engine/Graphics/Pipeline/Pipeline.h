#ifndef	PIPELINE_SIGNATURE_H
#define PIPELINE_SIGNATURE_H

#include "../../CommonDifinition.h"

class Pipeline
{
public:
	Pipeline();
	~Pipeline();

	/**
	* @brief �p�C�v���C���̏������֐�
	* @param[in] device �f�o�C�X
	* @param[in] root_signature ���[�g�V�O�l�`��
	* @retval true ����������
	* @retval false ���������s
	*/
	bool Initialize(ComPtr<ID3D12Device> device, ComPtr<ID3D12RootSignature> root_signature);

	/**
	* @brief �p�C�v���C���̉���֐�
	*/
	void Finalize();

	/**
	* @brief �p�C�v���C���̎擾�֐�
	* @retval �C���X�^���X
	*/
	ID3D12PipelineState* Get()
	{
		return this->instance.Get();
	}

private:
	ComPtr<ID3D12PipelineState> instance;
};

#endif