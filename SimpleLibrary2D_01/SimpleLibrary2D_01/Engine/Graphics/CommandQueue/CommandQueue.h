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
	* @details �`����g�p���邽�߂̏��������s��
	* @retval true ����������
	* @retval false ���������s
	*/
	bool Initialize(ComPtr<ID3D12Device> device);
	void Finalize();

	inline ID3D12CommandQueue* Get()
	{
		return this->commandQueue.Get();
	}

private:
	ComPtr<ID3D12CommandQueue> commandQueue;
};

#endif