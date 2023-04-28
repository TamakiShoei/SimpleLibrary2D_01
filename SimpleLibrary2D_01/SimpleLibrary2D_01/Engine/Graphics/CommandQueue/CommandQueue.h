#ifndef COMMAND_QUEUE
#define	COMMAND_QUEUE

#include "../../CommonDifinition.h"

class CommandQueue
{
public:
	CommandQueue();
	~CommandQueue();

	/**
	* @brief �R�}���h�L���[�̏������֐�
	* @param[in] device �f�o�C�X
	* @retval true ����������
	* @retval false ���������s
	*/
	bool Initialize(ComPtr<ID3D12Device> device);

	/**
	* @brief �R�}���h�L���[�̉���֐�
	*/
	void Finalize();

	/**
	* @brief �R�}���h�L���[�̎擾�֐�
	* @retval �C���X�^���X
	*/
	inline ID3D12CommandQueue* Get()
	{
		return this->instance.Get();
	}

private:
	ComPtr<ID3D12CommandQueue> instance;
};

#endif