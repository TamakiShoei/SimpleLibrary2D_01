/**
* @file VertexBuffer.h
* @brief 頂点バッファクラスの宣言
*/
#ifndef VERTEX_BUFFER
#define VERTEX_BUFFER

#include <map>
#include "../CommonDifinition.h"

class VertexBuffer
{
public:
	VertexBuffer();
	~VertexBuffer();

	/**
	* @brief 頂点バッファの作成
	* @param[in] デバイス
	* @retval 頂点バッファのインスタンス
	*/
	ID3D12Resource* Create(ID3D12Device* device);
};

#endif