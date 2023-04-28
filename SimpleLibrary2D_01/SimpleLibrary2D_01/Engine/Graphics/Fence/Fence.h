#ifndef	FENCE_H
#define FENCE_H

#include "../../CommonDifinition.h"

class Fence
{
public:
	Fence();
	~Fence();

	/**
	* @brief フェンスの初期化関数
	* @param[in] device デバイス
	* @retval true 初期化成功
	* @retval false 初期化失敗
	*/
	bool Initialize(ComPtr<ID3D12Device> device);

	/**
	* @brief GPU処理待ち関数
	* @details 毎フレームの最後に呼び出す必要がある
	* @param[in] command_queue コマンドキュー
	*/
	void WaitForPreviousFrame(ComPtr<ID3D12CommandQueue> command_queue);

	void Finalize();

	/**
	* @brief フェンスの取得関数
	* @retval インスタンス
	*/
	inline ID3D12Fence* Get()
	{
		return this->instance.Get();
	}

private:
	ComPtr<ID3D12Fence> instance;
	UINT fenceValue;
	HANDLE fenceEvent;		//フェンスハンドル
};

#endif