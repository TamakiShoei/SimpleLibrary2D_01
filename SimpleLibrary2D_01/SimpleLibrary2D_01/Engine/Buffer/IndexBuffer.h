#ifndef INDEX_BUFFER
#define INDEX_BUFFER

#include <map>
#include "../CommonDifinition.h"

class IndexBuffer
{
public:
	IndexBuffer();
	~IndexBuffer();

	/**
	* @brief インデックスバッファの作成
	* @param[in] デバイス
	* @retval true 生成成功　
	* @retval false 生成失敗
	*/
	ID3D12Resource* Create(ID3D12Device* device);
};

#endif