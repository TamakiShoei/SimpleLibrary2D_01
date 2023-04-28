#ifndef	ROOT_SIGNATURE_H
#define ROOT_SIGNATURE_H

#include "../../CommonDifinition.h"

class RootSignature
{
public:
	RootSignature();
	~RootSignature();

	/**
	* @brief ルートシグネチャの初期化関数
	* @param[in] device デバイス
	* @retval true 初期化成功
	* @retval false 初期化失敗
	*/
	bool Initialize(ComPtr<ID3D12Device> device);

	/**
	* @brief ルートシグネチャの解放関数
	*/
	void Finalize();

	/**
	* @brief ルートシグネチャの取得関数
	* @retval インスタンス
	*/
	ID3D12RootSignature* Get()
	{
		return this->instance.Get();
	}

private:
	ComPtr<ID3D12RootSignature> instance;
};

#endif