/**
* @file Graphics.h
* @brief <pre>
* グラフィッククラスの宣言
* Engineクラスでインスタンスを作成するため、使用者が宣言する必要はない
*/
#ifndef GRAPHICS_H
#define	GRAPHICS_H

#include "CommonDifinition.h"

class Graphics
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

	void DrawRect(VECTOR lower_left, VECTOR upper_left, VECTOR upper_right, VECTOR lower_right);

	/**
	* @brief GPU待ち関数
	* @details CommandListの完了を検知する
	*/
	void WaitForPreviousFrame();

public:
	//フレームカウントは最低2から(フロントバッファ・バックバッファ)
	static const UINT frameCount = 2;

	//GPU同期オブジェクト
	UINT				frameIndex = 0;	//フレームインデックス
	HANDLE				fenceEvent;		//フェンスハンドル
	ComPtr<ID3D12Fence>	fence;			//フェンス(GPUと同期して実行完了待ちを行う)

	//パイプラインオブジェクト
	ComPtr<ID3D12Device>				device;
	ComPtr<IDXGISwapChain4>				swapChain;
	ComPtr<ID3D12Resource>				renderTargets[frameCount];
	ComPtr<ID3D12CommandAllocator>		commandAllocator;
	ComPtr<ID3D12CommandQueue>			commandQueue;
	ComPtr<ID3D12DescriptorHeap>		rtvHeap;
	ComPtr<ID3D12RootSignature>			rootSignature;
	ComPtr<ID3D12PipelineState>			pipelineState;
	ComPtr<ID3D12GraphicsCommandList>	commandList;
	UINT								rtvDescriptorSize = 0;
	// パイプラインステートとは、描画パイプラインの流れを指定する機能。
	// ・描画に使用する各種シェーダコードの設定
	// ・ラスタライズの設定
	// ・ブレンド方法の設定
	// ・頂点レイアウトの設定
	// ・使用するRootSignatureの設定
	// ・深度ステンシルの設定
	// このような描画の一連の流れを指定する。

	ComPtr<IDXGIFactory4> factory;
	ComPtr<IDXGIAdapter1> hardwareAdapter = nullptr;
	ComPtr<IDXGIAdapter1> adapter;

	D3D12_VIEWPORT viewport;
	D3D12_RECT scissorRect;
	D3D12_VERTEX_BUFFER_VIEW vbView = {};

private:
	/**
	* @brief ファクトリーの初期化関数
	* @details 描画を使用するための初期化を行う
	* @param[in] dxgi_factory_flags デバッグモードの指定フラグ
	* @retval true 初期化成功
	* @retval false 初期化失敗
	*/
	bool InitializeFactory(UINT& dxgi_factory_flags);

	/**
	* @brief アダプタの初期化関数
	* @details 描画を使用するための初期化を行う
	* @retval true 初期化成功
	* @retval false 初期化失敗
	*/
	bool InitializeAdapter();

	/**
	* @brief コマンドキューの初期化関数
	* @details 描画を使用するための初期化を行う
	* @retval true 初期化成功
	* @retval false 初期化失敗
	*/
	bool InitializeCommandQueue();

	/**
	* @brief スワップチェインの初期化関数
	* @details 描画を使用するための初期化を行う
	* @retval true 初期化成功
	* @retval false 初期化失敗
	*/
	bool InitializeSwapChain();

	/**
	* @brief フェンスの初期化関数
	* @details 描画を使用するための初期化を行う
	* @retval true 初期化成功
	* @retval false 初期化失敗
	*/
	bool InitializeFence();

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

	/**
	* @brief グラフィックスパイプラインの生成関数
	* @details 描画を使用するための初期化を行う
	* @retval true 初期化成功
	* @retval false 初期化失敗
	*/
	bool CreatePipeline();

private:	
	ID3D12Resource* vertBuff = nullptr;
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle;
	CD3DX12_RESOURCE_BARRIER barrier;
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
	UINT64	fenceValue;		//フェンス値
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc = {};
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
};


#endif