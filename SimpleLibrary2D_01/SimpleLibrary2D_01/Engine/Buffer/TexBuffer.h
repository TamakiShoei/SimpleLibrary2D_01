#ifndef TEX_BUFFER
#define TEX_BUFFER

#include <map>
#include "../CommonDifinition.h"

class TexBuffer
{
public:
	TexBuffer();
	~TexBuffer();

	/**
	* @brief テクスチャバッファの作成
	* @param[in] device デバイス
	* @param[in] metadata メタデータ
	* @param[in] img 画像生データ
	* @retval true 生成成功
	* @retval false 生成失敗
	*/
	bool Create(ID3D12Device* device, DirectX::TexMetadata metadata, const DirectX::Image* img);

	/**
	* @brief テクスチャーバッファの取得関数
	* @retval テクスチャーバッファデータ
	*/
	ID3D12Resource* Get();

private:
	ID3D12Resource* buffer;
};

#endif