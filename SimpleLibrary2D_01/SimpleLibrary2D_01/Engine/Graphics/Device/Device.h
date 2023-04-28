#ifndef	DEVICE_H
#define DEVICE_H

#include "../../CommonDifinition.h"

class Device
{
public:
	Device();
	~Device();

	/**
	* @brief デバイスの初期化関数
	* @param[in] factory ファクトリー
	* @retval true 初期化成功
	* @retval false 初期化失敗
	*/
	bool Initialize(ComPtr<IDXGIFactory4> factory);

	/**
	* @brief デバイスの解放関数
	*/
	void Finalize();

	/**
	* @brief デバイスの取得関数
	* @retval インスタンス
	*/
	inline ID3D12Device* Get()
	{
		return this->instance.Get();
	}

private:
	ComPtr<ID3D12Device> instance;
	std::vector<D3D_FEATURE_LEVEL> levels;
};

#endif	