/**
* @file BufferManager.h
* @brief <pre>
* バッファマネージャークラスの宣言
* シングルトン化されたオブジェクト
* </pre>
*/
#ifndef BUFFER_MANAGER_H
#define BUFFER_MANAGER_H

#include <map>
#include <vector>
#include "../CommonDifinition.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "TexBuffer.h"
#include "ConstantBuffer.h"

struct SpriteData
{
	DirectX::TexMetadata metadata;
	std::vector<ID3D12Resource*> vertBuff;
	ID3D12Resource* indexBuff;
	ID3D12Resource* texBuff;
	ID3D12Resource* constBuff;
};

class BufferManager
{
public:
	static BufferManager* Instance()
	{
		static BufferManager instance;
		return &instance;
	}

	/**
	* @brief キャンバスの作成関数
	* @param[in] metadata 画像のメタデータ
	* @param[in] img 画像の生データ
	* @param[in] device デバイス
	*/
	int CreateSprite(DirectX::TexMetadata metadata, const DirectX::Image* img, ID3D12Device* device);

	/**
	* @brief 頂点バッファの取得関数
	* @param[in] key キー値
	* @param[in] device デバイス
	*/
	ID3D12Resource* GetVertexBuffer(int key, ID3D12Device* device);

	/**
	* @brief インデックスバッファの取得関数
	* @param[in] key キー値
	* @retval インデックスバッファデータ
	*/
	ID3D12Resource* GetIndexBuffer(int key);

	/**
	* @brief インデックスバッファの取得関数
	* @param[in] key キー値
	* @retval テクスチャバッファデータ
	*/
	ID3D12Resource* GetTexBuffer(int key);

	/**
	* @brief インデックスバッファの取得関数
	* @param[in] key キー値
	* @retval コンスタントバッファデータ
	*/
	ID3D12Resource* GetConstantBuffer(int key);

	/**
	* @brief メタデータの取得関数
	* @param[in] key キー値
	* @retval メタデータ
	*/
	DirectX::TexMetadata GetMetadata(int key);

	/**
	* @brief 頂点バッファクラスのUseCounterのリセット関数
	*/
	void ResetUseCounter();

	/**
	* @brief リソース解放関数
	*/
	void Finalize();

private:
	VertexBuffer vertBuff;
	IndexBuffer indexBuff;
	TexBuffer texBuff;
	ConstantBuffer constBuff;
	int key;
	std::map<int, SpriteData> spriteData;
	std::map<int, int> useCounter;

private:	//インスタンスの制限
	BufferManager() = default;
	~BufferManager() = default;

	BufferManager(const BufferManager&) = delete;
	BufferManager& operator = (const BufferManager&) = delete;
	BufferManager(BufferManager&&) = delete;
	BufferManager& operator = (BufferManager&&) = delete;
};

#endif