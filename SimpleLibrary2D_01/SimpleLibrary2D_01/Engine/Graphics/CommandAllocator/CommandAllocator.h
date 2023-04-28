#ifndef COMMAND_ALLOCATOR_H
#define	COMMAND_ALLOCATOR_H

#include "../../CommonDifinition.h"

class CommandAllocator
{
public:
	CommandAllocator();
	~CommandAllocator();

	/**
	* @brief コマンドアロケーターの初期化関数
	* @param[in] device デバイス
	* @retval true 初期化成功
	* @retval false 初期化失敗
	*/
	bool Initialize(ComPtr<ID3D12Device> device);

	/**
	* @brief コマンドアロケーターの解放関数
	*/
	void Finalize();

	/**
	* @brief コマンドアロケーターの取得関数
	* @retval インスタンス
	*/
	inline ID3D12CommandAllocator* Get()
	{
		return this->instance.Get();
	}

private:
	ComPtr<ID3D12CommandAllocator> instance;
};

#endif