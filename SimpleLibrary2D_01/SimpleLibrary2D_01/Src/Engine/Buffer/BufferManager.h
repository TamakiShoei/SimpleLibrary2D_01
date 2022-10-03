#ifndef BUFFER_MANAGER_H
#define BUFFER_MANAGER_H

#include <map>
#include "../CommonDifinition.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "TexBuffer.h"
#include "ConstantBuffer.h"

struct Buffers
{
	ID3D12Resource*	indexBuff;
	ID3D12Resource*	texBuff;
	ID3D12Resource*	constBuff;
	ID3D12Resource*	vertBuff;
};

struct TextureData
{
	DirectX::TexMetadata metadata;
	const DirectX::Image* img;
};

struct CanvasData
{
	TextureData texData;
	Buffers buffers;
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
	int CreateCanvas(DirectX::TexMetadata metadata, const DirectX::Image* img, ID3D12Device* device);

	/**
	* @brief 頂点バッファの取得関数
	* @param[in] key キー値
	* @retval 頂点バッファデータ
	*/
	ID3D12Resource* GetVertexBuffer(int key);

	/**
	* @brief インデックスバッファの取得関数
	* @param[in] key キー値
	* @retval インデックスバッファデータ
	*/
	ID3D12Resource* GetIndexBuffer(int key);

	/**
	* @brief テクスチャーバッファの取得関数
	* @param[in] key キー値
	* @retval テクスチャーバッファデータ
	*/
	ID3D12Resource* GetTexBuffer(int key);

	/**
	* @brief コンスタントバッファの取得関数
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

private:

	/**
	* @brief 頂点バッファの作成
	* @param[in] デバイス
	* @retval 頂点バッファデータ
	*/
	ID3D12Resource* CreateVertexBuffer(ID3D12Device* device);

	/**
	* @brief インデックスバッファの作成
	* @param[in] デバイス
	* @retval インデックスバッファデータ
	*/
	ID3D12Resource* CreateIndexBuffer(ID3D12Device* device);

	/**
	* @brief テクスチャバッファの作成
	* @param[in] デバイス
	* @retval テクスチャバッファデータ
	*/
	ID3D12Resource* CreateTexBuffer(ID3D12Device* device, DirectX::TexMetadata metadata, const DirectX::Image* img);

	/**
	* @brief コンスタントバッファの作成
	* @param[in] デバイス
	* @retval コンスタントバッファデータ
	*/
	ID3D12Resource* CreateConstantBuffer(ID3D12Device* device);

private:
	int key;
	static std::map<int, CanvasData> canvasData;

private:	//インスタンスの制限
	BufferManager() = default;
	~BufferManager() = default;

	BufferManager(const BufferManager&) = delete;
	BufferManager& operator = (const BufferManager&) = delete;
	BufferManager(BufferManager&&) = delete;
	BufferManager& operator = (BufferManager&&) = delete;
};

#endif