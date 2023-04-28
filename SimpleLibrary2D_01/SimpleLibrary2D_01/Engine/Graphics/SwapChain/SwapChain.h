#ifndef SWAP_CHAIN
#define	SWAP_CHAIN

#include "../../CommonDifinition.h"

class SwapChain
{
public:
	SwapChain();
	~SwapChain();

	/**
	* @brief スワップチェインの初期化関数
	* @param[in] factory ファクトリー
	* @param[in] command_queue コマンドキュー
	* @retval true 初期化成功
	* @retval false 初期化失敗
	*/
	bool Initialize(ComPtr<IDXGIFactory4> factory, ComPtr<ID3D12CommandQueue> command_queue);

	/**
	* @brief 描画先のバッファをバックバッファに設定
	*/
	void UpdateFrameIndex();

	/**
	* @brief スワップチェインの解放関数
	*/
	void Finalize();

	/**
	* @brief スワップチェインの取得関数
	* @retval インスタンス
	*/
	inline IDXGISwapChain4* Get()
	{
		return this->instance.Get();
	}

	/**
	* @brief 現在のフレームのインデックスを取得する関数
	*/
	inline UINT GetFrameIndex()
	{
		return this->frameIndex;
	}

private:
	ComPtr<IDXGISwapChain4> instance;
	UINT frameIndex;
};

#endif