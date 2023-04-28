#ifndef	ROOT_SIGNATURE_H
#define ROOT_SIGNATURE_H

#include "../../CommonDifinition.h"

class RootSignature
{
public:
	RootSignature();
	~RootSignature();

	/**
	* @brief ���[�g�V�O�l�`���̏������֐�
	* @param[in] device �f�o�C�X
	* @retval true ����������
	* @retval false ���������s
	*/
	bool Initialize(ComPtr<ID3D12Device> device);

	/**
	* @brief ���[�g�V�O�l�`���̉���֐�
	*/
	void Finalize();

	/**
	* @brief ���[�g�V�O�l�`���̎擾�֐�
	* @retval �C���X�^���X
	*/
	ID3D12RootSignature* Get()
	{
		return this->instance.Get();
	}

private:
	ComPtr<ID3D12RootSignature> instance;
};

#endif