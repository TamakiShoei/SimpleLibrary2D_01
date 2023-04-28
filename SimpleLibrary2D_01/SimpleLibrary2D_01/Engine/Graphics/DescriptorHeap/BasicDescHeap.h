#ifndef DESCRIPTOR_HEAP_H
#define	DESCRIPTOR_HEAP_H

#include <map>
#include "../../CommonDifinition.h"

class BasicDescHeap
{
public:
	BasicDescHeap();
	~BasicDescHeap();

	/**
	* @brief ディスクリプタヒープの初期化関数
	* @param[in] device デバイス
	* @retval true 初期化成功
	* @retval false 初期化失敗
	*/
	bool Initialize(ID3D12Device* device);

	/**
	* @brief ディスクリプタヒープにCBVを登録する関数
	* @param[in] cbv_desc CBVの設定
	* @param[in] key 画像のハンドル
	* @param[in] device デバイス
	*/
	void RegisterCBV(D3D12_CONSTANT_BUFFER_VIEW_DESC cbv_desc, int key, ID3D12Device* device);

	/**
	* @brief ディスクリプタヒープにSRVを登録する関数
	* @param[in] buff テクスチャバッファ
	* @param[in] srv_desc SRVの設定
	* @param[in] key 画像のハンドル
	* @param[in] device デバイス
	*/
	void RegisterSRV(ID3D12Resource* buff, D3D12_SHADER_RESOURCE_VIEW_DESC srv_desc, int key, ID3D12Device* device);
	
	/**
	* @brief CBVのハンドルを取得する関数
	* @param[in] key 画像のハンドル
	* @param[in] device デバイス
	* @retval CBVのハンドル
	*/
	D3D12_GPU_DESCRIPTOR_HANDLE GetCBVHandle(int key, ID3D12Device* device);

	/**
	* @brief SRVのハンドルを取得する関数
	* @param[in] key 画像のハンドル
	* @param[in] device デバイス
	* @retval SRVのハンドル
	*/
	D3D12_GPU_DESCRIPTOR_HANDLE GetSRVHandle(int key, ID3D12Device* device);

	/**
	* @brief ディスクリプタヒープの先頭アドレスを取得する関数
	* @retval ディスクリプタヒープの先頭アドレス
	*/
	ID3D12DescriptorHeap* Get();

	/**
	* @brief カウンターのリセット関数
	* @details <pre>
	* SRVやCBVの登録の際にカウンターを使ってアドレスの位置を決めているため
	* 毎フレームリセットをする必要がある
	* </pre>
	*/
	void ResetCounter();

	/**
	* @brief ディスクリプタヒープの解放関数
	*/
	void Finalize();

private:
	ComPtr<ID3D12DescriptorHeap> instance;
	D3D12_CPU_DESCRIPTOR_HANDLE handle;
	std::map<int, int> cbvDictionaly;
	std::map<int, int> srvDictionaly;
	int registerCounter;
};

#endif