#ifndef CONSTANT_BUFFER
#define CONSTANT_BUFFER

#include <map>
#include "../CommonDifinition.h"

class ConstantBuffer
{
public:
	ConstantBuffer();
	~ConstantBuffer();

	/**
	* @brief コンスタントバッファの作成
	* @param[in] デバイス
	* @retval true 生成成功
	* @retval false 生成失敗
	*/
	bool Create(ID3D12Device* device);

	/**
	* @brief コンスタントバッファの取得関数
	* @retval コンスタントバッファデータ
	*/
	ID3D12Resource* Get();

private:
	ID3D12Resource* buffer;
};

#endif