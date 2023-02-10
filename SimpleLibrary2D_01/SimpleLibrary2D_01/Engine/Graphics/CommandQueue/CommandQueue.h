#ifndef COMMAND_QUEUE
#define	COMMAND_QUEUE

#include "../../CommonDifinition.h"

class CommandQueue
{
public:
	CommandQueue();
	~CommandQueue();

	/**
	* @brief コマンドキューの初期化関数
	* @details 描画を使用するための初期化を行う
	* @retval true 初期化成功
	* @retval false 初期化失敗
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