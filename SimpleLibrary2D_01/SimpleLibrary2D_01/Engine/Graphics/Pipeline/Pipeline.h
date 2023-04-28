#ifndef	PIPELINE_SIGNATURE_H
#define PIPELINE_SIGNATURE_H

#include "../../CommonDifinition.h"

class Pipeline
{
public:
	Pipeline();
	~Pipeline();

	/**
	* @brief パイプラインの初期化関数
	* @param[in] device デバイス
	* @param[in] root_signature ルートシグネチャ
	* @retval true 初期化成功
	* @retval false 初期化失敗
	*/
	bool Initialize(ComPtr<ID3D12Device> device, ComPtr<ID3D12RootSignature> root_signature);

	/**
	* @brief パイプラインの解放関数
	*/
	void Finalize();

	/**
	* @brief パイプラインの取得関数
	* @retval インスタンス
	*/
	ID3D12PipelineState* Get()
	{
		return this->instance.Get();
	}

private:
	ComPtr<ID3D12PipelineState> instance;
};

#endif