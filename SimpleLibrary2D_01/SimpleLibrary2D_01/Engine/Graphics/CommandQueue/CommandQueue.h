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
	* @param[in] device デバイス
	* @retval true 初期化成功
	* @retval false 初期化失敗
	*/
	bool Initialize(ComPtr<ID3D12Device> device);

	/**
	* @brief コマンドキューの解放関数
	*/
	void Finalize();

	/**
	* @brief コマンドキューの取得関数
	* @retval インスタンス
	*/
	inline ID3D12CommandQueue* Get()
	{
		return this->instance.Get();
	}

private:
	ComPtr<ID3D12CommandQueue> instance;
};

#endif