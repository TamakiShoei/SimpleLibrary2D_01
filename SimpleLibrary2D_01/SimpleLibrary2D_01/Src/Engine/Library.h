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

		//�E�B���h�E�̏�����
		HRESULT Initialize();
		//VOID Finalize();

	public:
		//ComPtr�̓X�}�[�g�|�C���^�Ȃ̂ł��������������J������K�v���Ȃ��B
		//�t���[���J�E���g�͍Œ�2����(�t�����g�o�b�t�@�E�o�b�N�o�b�t�@)
		static const UINT frameCount = 2;

		//GPU�����I�u�W�F�N�g
		UINT				frameIndex = 0;	//�t���[���C���f�b�N�X
		HANDLE				fenceEvent;		//�t�F���X�n���h��
		ComPtr<ID3D12Fence>	fence;			//�t�F���X(GPU�Ɠ������Ď��s�����҂����s��)
		UINT64				fenceValue;		//�t�F���X�l

		//�p�C�v���C���I�u�W�F�N�g
		ComPtr<ID3D12Device>				device;
		ComPtr<IDXGISwapChain3>				swapChain;
		ComPtr<ID3D12Resource>				renderTargets[frameCount];
		ComPtr<ID3D12CommandAllocator>		commandAllocator;
		ComPtr<ID3D12CommandQueue>			commandQueue;
		ComPtr<ID3D12DescriptorHeap>		rtvHeap;
		ComPtr<ID3D12PipelineState>			pipelineState;
		ComPtr<ID3D12GraphicsCommandList>	commandList;
		UINT								rtvDescriptorSize = 0;
		// �p�C�v���C���X�e�[�g�Ƃ́A�`��p�C�v���C���̗�����w�肷��@�\�B
		// �E�`��Ɏg�p����e��V�F�[�_�R�[�h�̐ݒ�
		// �E���X�^���C�Y�̐ݒ�
		// �E�u�����h���@�̐ݒ�
		// �E���_���C�A�E�g�̐ݒ�
		// �E�g�p����RootSignature�̐ݒ�
		// �E�[�x�X�e���V���̐ݒ�
		// ���̂悤�ȕ`��̈�A�̗�����w�肷��B

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

		//�t���[���㏈��
		VOID WaitForPreviousFrame();
		//���
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
