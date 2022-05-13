#include "Graphics.h"

HRESULT Graphics::Initialize()
{
	UINT dxgiFactoryFlags = 0;
	InitializeFactory(dxgiFactoryFlags);

#if defined(_DEBUG)
	//DirectX12�̃f�o�b�O���C���[��L��
	{
		ComPtr<ID3D12Debug> debugController;
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(debugController.GetAddressOf()))))
		{
			debugController->EnableDebugLayer();

			//�ǉ��̃f�o�b�O���C���[��L��
			dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
		}
	}
#endif

	InitializeAdapter();
	InitializeCommandQueue();
	InitializeSwapChain();

	//Alt+Enter�ɂ��S��ʑJ�ڂ��ł��Ȃ��悤�ɂ���
	if (FAILED(factory->MakeWindowAssociation(FindWindow(WINDOW_TITLE, nullptr), DXGI_MWA_NO_ALT_ENTER)))
	{
		MessageBox(NULL, L"��ʂ̐ݒ肪�ł��܂���B", WINDOW_TITLE, MB_OK | MB_ICONERROR);
		return E_FAIL;
	}

	// �f�B�X�N���v�^�q�[�v�Ƃ����̂́AGPU��ɍ����f�X�N���v�^��ۑ����邽�߂̔z��B
	// GPU��������ɑ��݂���A�l�X�ȃf�[�^��o�b�t�@�̎�ނ�ʒu�A�傫���������\���̂̂悤�Ȃ��́B
	// ���炩�̃f�[�^�z��Ƃ��ĕ\����Ă���Ƃ������ƂɂȂ�B
	// ���̂悤�ɖ����I�ɋ�؂邱�Ƃɂ���āA���̒��̍\���̂̂悤�Ȕz�񂩂�f�[�^���Q�Ƃ��₷�����Ă���B
	InitializeRtvHeapDesc();
	CreateRenderTargetView();

	// �R�}���h�A���P�[�^�[���쐬
	if (FAILED(device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(commandAllocator.GetAddressOf()))))
	{
		MessageBox(NULL, L"�R�}���h�A���P�[�^���쐬�ł��܂���ł����B", WINDOW_TITLE, MB_OK | MB_ICONERROR);
		return E_FAIL;
	}

	// �R�}���h���X�g���쐬
	if (FAILED(device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator.Get(), nullptr, IID_PPV_ARGS(commandList.GetAddressOf()))))
	{
		MessageBox(NULL, L"�R�}���h���X�g���쐬�ł��܂���ł����B", WINDOW_TITLE, MB_OK | MB_ICONERROR);
		return E_FAIL;
	}

	// �R�}���h���X�g���N���[�Y
	commandList->Close();
	// �R�}���h���X�g�͋L�^��Ԃō쐬����邪�A����͏��������ł����ɉ�������Ȃ��̂ł����ɕ���B

	InitializeFance();

	return S_OK;
}

HRESULT Graphics::InitializeFactory(UINT& dxgiFactoryFlags)
{
	//�t�@�N�g�����쐬
	if (FAILED(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(factory.GetAddressOf()))))
	{
		MessageBox(NULL, L"�t�@�N�g�����쐬�ł��܂���ł����B", WINDOW_TITLE, MB_OK | MB_ICONERROR);
		return E_FAIL;
	}
}

HRESULT Graphics::InitializeAdapter()
{
	//DirectX12���T�|�[�g���闘�p�\�ȃn�[�h�E�F�A�A�_�v�^���������擾
	HRESULT hr;
	for (UINT adapterIndex = 0; DXGI_ERROR_NOT_FOUND != factory->EnumAdapters1(adapterIndex, adapter.GetAddressOf()); ++adapterIndex)
	{
		DXGI_ADAPTER_DESC1 adapterDesc;
		adapter->GetDesc1(&adapterDesc);

		//�ǂ��炩��FALSE�Ȃ�FALSE�ŃX���[
		if (adapterDesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) continue;

		//�A�_�v�^��DirectX12�ɑΉ����Ă��邩�m�F
		hr = D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), nullptr);
		if (SUCCEEDED(hr))
		{
			//�f�o�C�X���쐬
			if (FAILED(D3D12CreateDevice(hardwareAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(Graphics::device.GetAddressOf()))))
			{
				MessageBox(NULL, L"�I�������n�[�h�E�F�A�f�o�C�X��DIrectX12�ɑΉ����Ă��܂���B", WINDOW_TITLE, MB_OK | MB_ICONERROR);
				return E_FAIL;
			}
			break;
		}
	}

	//�֘A�t������
	hardwareAdapter = adapter.Detach();

	//�n�[�h�E�F�A�őΉ��ł��Ȃ��ꍇ��WARP�f�o�C�X(�\�t�g�E�F�A)�̕���p����
	if (FAILED(hr))
	{
		ComPtr<IDXGIAdapter> warpAdapter;
		factory->EnumWarpAdapter(IID_PPV_ARGS(warpAdapter.GetAddressOf()));
		if (FAILED(D3D12CreateDevice(warpAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(Graphics::device.GetAddressOf()))))
		{
			MessageBox(NULL, L"�I������WARP�f�o�C�X�܂ł���DirectX12�ɑΉ����Ă��܂���B", WINDOW_TITLE, MB_OK | MB_ICONERROR);
			return E_FAIL;
		}
	}
}

HRESULT Graphics::InitializeCommandQueue()
{
	//�R�}���h�L���[�̐ݒ�
	commandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	commandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

	//�R�}���h�L���[���쐬
	if (FAILED(device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(commandQueue.GetAddressOf()))))
	{
		MessageBox(NULL, L"�R�}���h�L���[���쐬�ł��܂���ł����B", WINDOW_TITLE, MB_OK | MB_ICONERROR);
		return E_FAIL;
	}
}

HRESULT Graphics::InitializeSwapChain()
{
	//�X���b�v�`�F�C���̍쐬
	swapChainDesc.BufferCount = frameCount;
	swapChainDesc.Width = WINDOW_WIDTH;
	swapChainDesc.Height = WINDOW_HEIGHT;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.SampleDesc.Count = 1;
	//�X���b�v�`�F�C���Ƃ́A�����_�����O���ʂ��o�͂��邽�߂̃I�u�W�F�N�g
	//�R�Â����r�f�I�A�_�v�^��E�B���h�E�ɑ΂��ă����_�����O���ʂ��o�͂���

	//�X���b�v�`�F�C�����쐬
	ComPtr<IDXGISwapChain1>	tmpSwapChain;
	if (FAILED(factory->CreateSwapChainForHwnd(commandQueue.Get(), FindWindow(WINDOW_TITLE, nullptr), &swapChainDesc, nullptr, nullptr, tmpSwapChain.GetAddressOf())))
	{
		MessageBox(NULL, L"�X���b�v�`�F�C�����쐬�ł��܂���ł����B", WINDOW_TITLE, MB_OK | MB_ICONERROR);
		return E_FAIL;
	}

	//�X���b�v�`�F�C�����L���X�g
	tmpSwapChain.As(&swapChain);

	//�o�b�N�o�b�t�@�̃C���f�b�N�X���i�[
	frameIndex = swapChain->GetCurrentBackBufferIndex();
}

HRESULT Graphics::InitializeFance()
{
	// �t�F���X���쐬
	if (FAILED(Graphics::device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(Graphics::fence.GetAddressOf()))))
	{
		MessageBox(NULL, L"�t�F���X���쐬�ł��܂���ł����B", WINDOW_TITLE, MB_OK | MB_ICONERROR);
		return E_FAIL;
	}
	// �t�F���X�Ƃ́A�����I�u�W�F�N�g�Ƃ��ă��\�[�X��GPU�ɃA�b�v���[�h�����܂őҋ@������́B

	// �t�F���X�l��1�ɐݒ�
	Graphics::fenceValue = 1;

	// �t���[�������Ɏg�p����C�x���g�n���h�����쐬
	Graphics::fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	if (Graphics::fenceEvent == nullptr)
	{
		MessageBox(NULL, L"�t�F���X�C�x���g�n���h�����쐬�ł��܂���ł����B", WINDOW_TITLE, MB_OK | MB_ICONERROR);
		return E_FAIL;
	}
	// �҂ۂ�Windows��Event�𗘗p
}

HRESULT Graphics::InitializeRtvHeapDesc()
{
	//�����_�[�^�[�Q�b�g�r���[�p�̃f�B�X�N���v�^�[�q�[�v�̐ݒ�
	rtvHeapDesc.NumDescriptors = Graphics::frameCount;
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	// �f�B�X�N���v�^�Ƃ́AGPU�ƃ��\�[�X�̋��n�����s�������̂��́B
	// DX11�܂ł͉B������Ă����B
	// �f�B�X�N���v�^�ɂ�3�̊T�O�����݂���B
	// Descriptor�F�e�N�X�`���Ȃǂ̃��\�[�X��GPU�ƕR�Â���B
	// DescriptorHeap�FDescriptorHeap����Descriptor���쐬����B�Ǘ��ł���Descriptor�̎�ނ␔�͎��O�Ɏw��B
	// DescriptorTable�FGPU��Ŏg�p����Descriptor�̐���z�u�𐧌䂷��BDescriptorTable��RootSignature�Őݒ肷��B

	// �f�B�X�N���v�^�q�[�v���쐬
	if (FAILED(Graphics::device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(Graphics::rtvHeap.GetAddressOf()))))
	{
		MessageBox(NULL, L"�f�B�X�N���v�^�q�[�v���쐬�ł��܂���ł����B", WINDOW_TITLE, MB_OK | MB_ICONERROR);
		return E_FAIL;
	}

	//�f�B�X�N���v�^�̃T�C�Y���擾
	Graphics::rtvDescriptorSize = Graphics::device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

}

HRESULT Graphics::CreateRenderTargetView()
{
	//�t���[�����\�[�X�̃n���h�����擾
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(rtvHeap->GetCPUDescriptorHandleForHeapStart());

	// �t���[���o�b�t�@�ƃo�b�N�o�b�t�@�̃����_�[�^�[�Q�b�g�r���[���쐬
	for (UINT i = 0; i < frameCount; i++)
	{
		// RTV�o�b�t�@���擾
		if (FAILED(swapChain->GetBuffer(i, IID_PPV_ARGS(renderTargets[i].GetAddressOf()))))
		{
			MessageBox(NULL, L"RTV�o�b�t�@���擾�ł��܂���ł����B", WINDOW_TITLE, MB_OK | MB_ICONERROR);
			return E_FAIL;
		}

		// �����_�[�^�[�Q�b�g�r���[���쐬
		device->CreateRenderTargetView(renderTargets[i].Get(), nullptr, rtvHandle);

		// �n���h���̃I�t�Z�b�g
		rtvHandle.Offset(1, rtvDescriptorSize);
	}
}
