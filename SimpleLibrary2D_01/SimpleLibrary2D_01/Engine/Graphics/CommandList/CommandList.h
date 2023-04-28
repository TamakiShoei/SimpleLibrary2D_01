#ifndef	COMMAND_LIST_H
#define COMMAND_LIST_H

#include "../../CommonDifinition.h"

class CommandList
{
public:
	CommandList();
	~CommandList();

	/**
	* @brief �R�}���h���X�g�̏������֐�
	* @param[in] device �f�o�C�X
	* @param[in] command_allocator �R�}���h�A���P�[�^�[
	* @retval true ����������
	* @retval false ���������s
	*/
	bool Initialize(ComPtr<ID3D12Device> device, ComPtr<ID3D12CommandAllocator> command_allocator);

	/**
	* @brief �R�}���h���X�g�̉���֐�
	*/
	void Finalize();

	/**
	* @brief �R�}���h���X�g�̎擾�֐�
	* @retval �C���X�^���X
	*/
	inline ID3D12GraphicsCommandList* Get()
	{
		return this->instance.Get();
	}

private:
	ComPtr<ID3D12GraphicsCommandList> instance;
};

#endif