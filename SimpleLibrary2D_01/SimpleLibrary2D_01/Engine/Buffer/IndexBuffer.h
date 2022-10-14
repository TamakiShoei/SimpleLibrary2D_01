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
	bool Create(ID3D12Device* device);

	/**
	* @brief インデックスバッファの取得関数
	* @retval インデックスバッファデータ
	*/
	ID3D12Resource* Get();

private:
	ID3D12Resource* buffer;
};

#endif