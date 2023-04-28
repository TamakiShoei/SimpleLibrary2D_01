#ifndef	FENCE_H
#define FENCE_H

#include "../../CommonDifinition.h"

class Fence
{
public:
	Fence();
	~Fence();

	/**
	* @brief �t�F���X�̏������֐�
	* @param[in] device �f�o�C�X
	* @retval true ����������
	* @retval false ���������s
	*/
	bool Initialize(ComPtr<ID3D12Device> device);

	/**
	* @brief GPU�����҂��֐�
	* @details ���t���[���̍Ō�ɌĂяo���K�v������
	* @param[in] command_queue �R�}���h�L���[
	*/
	void WaitForPreviousFrame(ComPtr<ID3D12CommandQueue> command_queue);

	void Finalize();

	/**
	* @brief �t�F���X�̎擾�֐�
	* @retval �C���X�^���X
	*/
	inline ID3D12Fence* Get()
	{
		return this->instance.Get();
	}

private:
	ComPtr<ID3D12Fence> instance;
	UINT fenceValue;
	HANDLE fenceEvent;		//�t�F���X�n���h��
};

#endif