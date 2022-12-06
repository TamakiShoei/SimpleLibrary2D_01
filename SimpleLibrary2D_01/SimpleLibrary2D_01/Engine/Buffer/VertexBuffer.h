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
	* @retval true 生成成功
	* @retval false 生成失敗
	*/
	ID3D12Resource* Create(ID3D12Device* device);
};

#endif