/**
* @file TexBuffer.h
* @brief テクスチャバッファークラスの宣言
*/
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
	* @retval テクスチャバッファのインスタンス
	*/
	ID3D12Resource* Create(ID3D12Device* device, DirectX::TexMetadata metadata, const DirectX::Image* img);
};

#endif