﻿/**
* @file Graphics.h
* @brief <pre>
* グラフィッククラスの宣言
* Engineクラスでインスタンスを作成するため、使用者が宣言する必要はない
* </pre>
*/
#ifndef GRAPHICS_H
#define	GRAPHICS_H

#include <map>
#include "../CommonDifinition.h"
#include "../Buffer/BufferManager.h"
#include "Factory/Factory.h"
#include "Device/Device.h"
#include "CommandQueue/CommandQueue.h"
#include "SwapChain/SwapChain.h"
#include "RootSignature/RootSignature.h"
#include "Pipeline/Pipeline.h"
#include "CommandAllocator/CommandAllocator.h"
#include "CommandList/CommandList.h"
#include "Fence/Fence.h"
#include "DescriptorHeap/BasicDescHeap.h"

class Graphics : VertexBuffer
{
public:
	Graphics()
	{
	}
	~Graphics()
	{
	}

	/**
	* @brief Graphicsクラスの初期化関数
	* @details 描画を使用するための初期化を行う
	* @retval true 初期化成功
	* @retval false 初期化失敗
	*/
	bool Initialize();

	/**
	* @brief 描画開始関数
	* @details <pre>
	* バックバッファのクリアを行う
	* この関数は必ず描画処理前に実行する必要がある
	* また、実行後にClearScreen関数を実行する必要がある
	* </pre>
	*/
	void ClearScreen();

	/**
	* @brief 描画終了関数
	* @details <pre>
	* バックバッファとフロントバッファを入れ替える
	* この関数は必ずClearScreenの後に実行する必要がある
	* </pre>
	*/
	void ScreenFlip();
	
	/**
	* @brief 三角形描画関数
	* @details 引数で受け取った頂点をもとに三角形を描画する
	* @param[in] lower_left 左下頂点座標
	* @param[in] upper_left 左上頂点座標
	* @param[in] lower_right 右下頂点座標
	*/
	void DrawTriangle(VECTOR lower_left, VECTOR upper_left, VECTOR lower_right);

	/**
	* @brief 矩形描画関数
	* @details 引数で受け取った頂点をもとに三角形を描画する
	* @param[in] lower_left 左下頂点座標
	* @param[in] upper_left 左上頂点座標
	* @param[in] upper_right 右上頂点座標
	* @param[in] lower_right 右下頂点座標
	*/
	void DrawRect(VECTOR lower_left, VECTOR upper_left, VECTOR upper_right, VECTOR lower_right);

	/**
	* @brief 画像読み込み関数
	* @details 引数で受け取ったファイルパスをもとに画像を描画する際に必要なキーを返す
	* @param[in] file_path 読み込みたい画像のファイルパス
	* @retval 画像のキー値
	*/
	int LoadTexture(const char* file_path);

	/**
	* @brief テクスチャ描画関数
	* @details 引数で受け取った頂点をもとにテクスチャを描画する
	* @param[in] pos_x 左下頂点x座標
	* @param[in] pos_y 左上頂点y座標
	* @param[in] key 描画する画像のキー値(ハンドル)
	*/
	void DrawTexture(float pos_x, float pos_y, int key);

	/**
	* @brief GPU待ち関数
	* @details CommandListの完了を検知する
	*/
	void WaitForPreviousFrame();

	/**
	* @brief リソース解放関数
	*/
	void Finalize();

private:

	/**
	* @brief シザー矩形とビューポートの設定
	* @details 描画をする範囲を設定する
	*/
	void SetDrawArea();

	/**
	* @brief レンダーターゲットビューのディスクリプタヒープの生成関数
	* @details 描画を使用するための初期化を行う
	* @retval true 初期化成功
	* @retval false 初期化失敗
	*/
	bool CreateRtvDescHeap();

	/**
	* @brief レンダーターゲットビューの生成関数
	* @details 描画を使用するための初期化を行う
	* @retval true 初期化成功
	* @retval false 初期化失敗
	*/
	bool CreateRenderTargetView();

private:	
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle;
	CD3DX12_RESOURCE_BARRIER barrier;
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc = {};
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
	UINT64	fenceValue;		//フェンス値

	//フレームカウントは最低2から(フロントバッファ・バックバッファ)
	static const UINT frameCount = 2;

	//GPU同期オブジェクト
	UINT				frameIndex = 0;	//フレームインデックス
	HANDLE				fenceEvent;		//フェンスハンドル
	//ComPtr<ID3D12Fence>	fence;			//フェンス(GPUと同期して実行完了待ちを行う)

	//パイプラインオブジェクト
	Factory factory;
	Device								device;
	CommandQueue						commandQueue;
	SwapChain							swapChain;
	RootSignature						rootSignature;
	Pipeline							pipeline;
	CommandAllocator					commandAllocator;
	CommandList							commandList;
	Fence								fence;
	ComPtr<ID3D12Resource>				renderTargets[frameCount];
	ComPtr<ID3D12DescriptorHeap>		rtvHeap;
	BasicDescHeap						basicHeap;
	UINT								rtvDescriptorSize = 0;

	D3D12_VIEWPORT viewport;
	D3D12_RECT scissorRect;
};


#endif