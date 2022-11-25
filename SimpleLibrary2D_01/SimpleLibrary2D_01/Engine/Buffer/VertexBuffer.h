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
	void Create(ID3D12Device* device);

	/**
	* @brief 頂点バッファの取得関数
	* @retval 頂点バッファデータ
	*/
	ID3D12Resource* Get(ID3D12Device* device);

	/**
	* @brief 頂点バッファ解放関数
	*/
	void Finalize();

	/**
	* @brief UseCounterのリセット関数
	* @details ClearScreenで必ず毎フレーム呼ぶ
	*/
	void ResetUseCounter();

private:
	std::vector<ID3D12Resource*> buffers;
	int useCounter;
};

#endif