#ifndef COMMAND_ALLOCATOR_H
#define	COMMAND_ALLOCATOR_H

#include "../../CommonDifinition.h"

class CommandAllocator
{
public:
	CommandAllocator();
	~CommandAllocator();

	/**
	* @brief �R�}���h�A���P�[�^�[�̏������֐�
	* @param[in] device �f�o�C�X
	* @retval true ����������
	* @retval false ���������s
	*/
	bool Initialize(ComPtr<ID3D12Device> device);

	/**
	* @brief �R�}���h�A���P�[�^�[�̉���֐�
	*/
	void Finalize();

	/**
	* @brief �R�}���h�A���P�[�^�[�̎擾�֐�
	* @retval �C���X�^���X
	*/
	inline ID3D12CommandAllocator* Get()
	{
		return this->instance.Get();
	}

private:
	ComPtr<ID3D12CommandAllocator> instance;
};

#endif