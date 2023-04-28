#ifndef	COMMAND_LIST_H
#define COMMAND_LIST_H

#include "../../CommonDifinition.h"

class CommandList
{
public:
	CommandList();
	~CommandList();

	/**
	* @brief コマンドリストの初期化関数
	* @param[in] device デバイス
	* @param[in] command_allocator コマンドアロケーター
	* @retval true 初期化成功
	* @retval false 初期化失敗
	*/
	bool Initialize(ComPtr<ID3D12Device> device, ComPtr<ID3D12CommandAllocator> command_allocator);

	/**
	* @brief コマンドリストの解放関数
	*/
	void Finalize();

	/**
	* @brief コマンドリストの取得関数
	* @retval インスタンス
	*/
	inline ID3D12GraphicsCommandList* Get()
	{
		return this->instance.Get();
	}

private:
	ComPtr<ID3D12GraphicsCommandList> instance;
};

#endif