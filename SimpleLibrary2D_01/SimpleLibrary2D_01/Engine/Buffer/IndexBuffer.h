/**
* @file IndexBuffer.h
* @brief インデックスバッファークラスの宣言
*/
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
	* @retval インデックスバッファのインスタンス
	*/
	ID3D12Resource* Create(ID3D12Device* device);
};

#endif