#ifndef LIBRARY_H 
#define	LIBRARY_H

#include "CommonDifinition.h"

namespace Library
{
	class Graphics
	{
	public:
		Graphics()
		{
		}
		~Graphics()
		{
		}

		//ウィンドウの初期化
		HRESULT Initialize();
		//VOID Finalize();

	public:
		//ComPtrはスマートポインタなのでいちいちメモリ開放する必要がない。
		//フレームカウントは最低2から(フロントバッファ・バックバッファ)
		static const UINT frameCount = 2;

		//GPU同期オブジェクト
		UINT				frameIndex = 0;	//フレームインデックス
		HANDLE				fenceEvent;		//フェンスハンドル
		ComPtr<ID3D12Fence>	fence;			//フェンス(GPUと同期して実行完了待ちを行う)
		UINT64				fenceValue;		//フェンス値

		//パイプラインオブジェクト
		ComPtr<ID3D12Device>				device;
		ComPtr<IDXGISwapChain3>				swapChain;
		ComPtr<ID3D12Resource>				renderTargets[frameCount];
		ComPtr<ID3D12CommandAllocator>		commandAllocator;
		ComPtr<ID3D12CommandQueue>			commandQueue;
		ComPtr<ID3D12DescriptorHeap>		rtvHeap;
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

	private:
		HRESULT InitializeFactory(UINT& dxgiFactoryFlags);
		HRESULT InitializeAdapter();
		HRESULT InitializeCommandQueue();
		HRESULT InitializeSwapChain();
		HRESULT InitializeFance();
		HRESULT InitializeRtvHeapDesc();

		HRESULT CreateRenderTargetView();

	private:
		D3D12_COMMAND_QUEUE_DESC queueDesc = {};
		DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
		D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
	};

	class Windows
	{
	public:
		Windows() : isClosed(false), isRecievedMessage(false)
		{
		}

		static LRESULT CALLBACK WindowProc(HWND hwnd, UINT nMsg, WPARAM wParam, LPARAM lParam);

		HRESULT InitializeWindows();
		
		void Update();

		bool IsClosed()
		{
			return isClosed;
		}

		/*bool IsRecievedMessage()
		{
			return isRecievedMessage;
		}*/

	private:
		bool isClosed;
		bool isRecievedMessage;
	};

	class Render
	{
	public:
		Render()
		{
		}

		~Render()
		{
		}

		VOID ClearScreen();
		VOID ScreenFlip();
	};

	class Engine
	{
	public:
		static Engine* Instance()
		{
			static Engine instance;
			return &instance;
		}

		HRESULT OnInit();
		VOID End();

	public:

		//フレーム後処理
		VOID WaitForPreviousFrame();
		//解放
		VOID OnDestroy();

		Windows* GetWindows()
		{
			return &windows;
		}

		Render* GetRender()
		{
			return &render;
		}

		Graphics* GetGraphics()
		{
			return &graphics;
		}

		Windows windows;
		Render render;
		Graphics graphics;
	};
}
#endif
