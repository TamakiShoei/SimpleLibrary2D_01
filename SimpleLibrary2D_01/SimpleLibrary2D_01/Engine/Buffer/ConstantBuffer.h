/**
* @file ConstantBuffer.h
* @brief コンスタントバッファークラスの宣言
*/

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
	* @retval コンスタントバッファのインスタンス
	*/
	ID3D12Resource* Create(ID3D12Device* device);
};

#endif