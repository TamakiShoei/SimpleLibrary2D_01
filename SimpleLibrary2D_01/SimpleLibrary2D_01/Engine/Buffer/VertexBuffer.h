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
	bool Create(ID3D12Device* device);

	/**
	* @brief 頂点バッファの取得関数
	* @retval 頂点バッファデータ
	*/
	ID3D12Resource* Get();

private:
	ID3D12Resource* buffer;
};

#endif