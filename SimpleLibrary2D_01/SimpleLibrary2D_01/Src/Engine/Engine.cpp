#include "Library.h"

HRESULT Library::Engine::OnInit()
{
	if (FAILED(Library::Engine::Instance()->GetWindows()->InitWindows()))
	{
		return E_FAIL;
	}

	UINT dxgiFactoryFlags = 0;
	InitFactory(dxgiFactoryFlags);

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

	InitAdapter();
	InitCommandQueue();

	//Alt+Enter�ɂ��S��ʑJ�ڂ��ł��Ȃ��悤�ɂ���
	if (FAILED(factory->MakeWindowAssociation(FindWindow(WINDOW_TITLE, nullptr), DXGI_MWA_NO_ALT_ENTER)))
	{
		MessageBox(NULL, L"��ʂ̐ݒ肪�ł��܂���B", WINDOW_TITLE, MB_OK | MB_ICONERROR);
		return E_FAIL;
	}

	//�X���b�v�`�F�C���̍쐬
	swapChainDesc.BufferCount = Engine::Instance()->frameCount;
	swapChainDesc.Width = WINDOW_WIDTH;
	swapChainDesc.Height = WINDOW_HEIGHT;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.SampleDesc.Count = 1;
	//�X���b�v�`�F�C���Ƃ́A�����_�����O���ʂ��o�͂��邽�߂̃I�u�W�F�N�g
	//�R�Â����r�f�I�A�_�v�^��E�B���h�E�ɑ΂��ă����_�����O���ʂ��o�͂���

	//�X���b�v�`�F�C�����쐬
	ComPtr<IDXGISwapChain1> swapChain;
	if (FAILED(factory->CreateSwapChainForHwnd(Engine::Instance()->commandQueue.Get(), FindWindow(WINDOW_TITLE, nullptr), &swapChainDesc, nullptr, nullptr, swapChain.GetAddressOf())))
	{
		MessageBox(NULL, L"�X���b�v�`�F�C�����쐬�ł��܂���ł����B", WINDOW_TITLE, MB_OK | MB_ICONERROR);
		return E_FAIL;
	}

	//�X���b�v�`�F�C�����L���X�g
	swapChain.As(&Engine::Instance()->swapChain);

	//�o�b�N�o�b�t�@�̃C���f�b�N�X���i�[
	Engine::Instance()->frameIndex = Engine::Instance()->swapChain->GetCurrentBackBufferIndex();

	// �f�B�X�N���v�^�q�[�v�Ƃ����̂́AGPU��ɍ����f�X�N���v�^��ۑ����邽�߂̔z��B
	// GPU��������ɑ��݂���A�l�X�ȃf�[�^��o�b�t�@�̎�ނ�ʒu�A�傫���������\���̂̂悤�Ȃ��́B
	// ���炩�̃f�[�^�z��Ƃ��ĕ\����Ă���Ƃ������ƂɂȂ�B
	// ���̂悤�ɖ����I�ɋ�؂邱�Ƃɂ���āA���̒��̍\���̂̂悤�Ȕz�񂩂�f�[�^���Q�Ƃ��₷�����Ă���B
	InitRtvHeapDesc();

	{
		//�t���[�����\�[�X�̃n���h�����擾
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(Engine::Instance()->rtvHeap->GetCPUDescriptorHandleForHeapStart());

		// �t���[���o�b�t�@�ƃo�b�N�o�b�t�@�̃����_�[�^�[�Q�b�g�r���[���쐬
		for (UINT i = 0; i < Engine::Instance()->frameCount; i++)
		{
			// RTV�o�b�t�@���擾
		if (FAILED(Engine::Instance()->swapChain->GetBuffer(i, IID_PPV_ARGS(Engine::Instance()->renderTargets[i].GetAddressOf()))))
			{
				MessageBox(NULL, L"RTV�o�b�t�@���擾�ł��܂���ł����B", WINDOW_TITLE, MB_OK | MB_ICONERROR);
				return E_FAIL;
			}

			// �����_�[�^�[�Q�b�g�r���[���쐬
			Engine::Instance()->device->CreateRenderTargetView(Engine::Instance()->renderTargets[i].Get(), nullptr, rtvHandle);

			// �n���h���̃I�t�Z�b�g
			rtvHandle.Offset(1, Engine::Instance()->rtvDescriptorSize);
		}
	}

	// �R�}���h�A���P�[�^�[���쐬
	if (FAILED(Engine::Instance()->device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(Engine::Instance()->commandAllocator.GetAddressOf()))))
	{
		MessageBox(NULL, L"�R�}���h�A���P�[�^���쐬�ł��܂���ł����B", WINDOW_TITLE, MB_OK | MB_ICONERROR);
		return E_FAIL;
	}

	// �R�}���h���X�g���쐬
	if (FAILED(Engine::Instance()->device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, Engine::Instance()->commandAllocator.Get(), nullptr, IID_PPV_ARGS(Engine::Instance()->commandList.GetAddressOf()))))
	{
		MessageBox(NULL, L"�R�}���h���X�g���쐬�ł��܂���ł����B", WINDOW_TITLE, MB_OK | MB_ICONERROR);
		return E_FAIL;
	}

	// �R�}���h���X�g���N���[�Y
	Engine::Instance()->commandList->Close();
	// �R�}���h���X�g�͋L�^��Ԃō쐬����邪�A����͏��������ł����ɉ�������Ȃ��̂ł����ɕ���B

	InitFance();

	return S_OK;
}



VOID Library::Render::ClearScreen()
{
	// �R�}���h�A���P�[�^�����Z�b�g
	Library::Engine::Instance()->commandAllocator->Reset();

	// �R�}���h���X�g�����Z�b�g
	Engine::Instance()->commandList->Reset(Engine::Instance()->commandAllocator.Get(), Engine::Instance()->pipelineState.Get());

	// �o�b�N�o�b�t�@�������_�[�^�[�Q�b�g�Ƃ��Ďg�p
	auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(Engine::Instance()->renderTargets[Engine::Instance()->frameIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	Engine::Instance()->commandList->ResourceBarrier(1, &barrier);
	// ���\�[�X�o���A�Ƃ́AGPU���ň������\�[�X�̏󋵂𓯊�������@�\�B
	// �}���`�X���b�h��O��Ƃ��������Ȃ̂ŁAGPU���̓���������̃A�N�Z�X�������ɍs���邱�Ƃ�z�肵���@�\���Ƃ������ƁB

	// �����_�[�^�[�Q�b�g�r���[�̃n���h�����쐬
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(Engine::Instance()->rtvHeap->GetCPUDescriptorHandleForHeapStart(), Engine::Instance()->frameIndex, Engine::Instance()->rtvDescriptorSize);

	// �o�b�N�o�b�t�@�ɕ`��(�R�}���h���L�^)
	const FLOAT	clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };		// ���ۂ��F
	Engine::Instance()->commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
}

VOID Library::Render::ScreenFlip()
{
	// �o�b�N�o�b�t�@��\��
	auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(Engine::Instance()->renderTargets[Engine::Instance()->frameIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	Engine::Instance()->commandList->ResourceBarrier(1, &barrier);

	// �R�}���h���X�g���N���[�Y
	Engine::Instance()->commandList->Close();

	// �R�}���h���X�g�����s
	ID3D12CommandList* ppCommandLists[] = { Engine::Instance()->commandList.Get() };
	Engine::Instance()->commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

	// �t���[�����ŏI�o��
	Engine::Instance()->swapChain->Present(1, 0);

	// �t���[���㏈��
	Library::Engine::Instance()->WaitForPreviousFrame();
}

VOID Library::Engine::WaitForPreviousFrame()
{
	const UINT64 fence = Engine::Instance()->fenceValue;
	Engine::Instance()->commandQueue->Signal(Engine::Instance()->fence.Get(), fence);
	Engine::Instance()->fenceValue++;

	// �O�̃t���[�����I������܂őҋ@
	if (Engine::Instance()->fence->GetCompletedValue() < fence) {
		Engine::Instance()->fence->SetEventOnCompletion(fence, Engine::Instance()->fenceEvent);
		WaitForSingleObject(Engine::Instance()->fenceEvent, INFINITE);
	}

	// �o�b�N�o�b�t�@�̃C���f�b�N�X���i�[
	Engine::Instance()->frameIndex = Engine::Instance()->swapChain->GetCurrentBackBufferIndex();
}

VOID Library::Engine::OnDestroy()
{
	WaitForPreviousFrame();
	CloseHandle(Engine::Instance()->fenceEvent);
}

HRESULT Library::Engine::InitFactory(UINT &dxgiFactoryFlags)
{
	//�t�@�N�g�����쐬
	if (FAILED(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(factory.GetAddressOf()))))
	{
		MessageBox(NULL, L"�t�@�N�g�����쐬�ł��܂���ł����B", WINDOW_TITLE, MB_OK | MB_ICONERROR);
		return E_FAIL;
	}
}

HRESULT Library::Engine::InitAdapter()
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
			if (FAILED(D3D12CreateDevice(hardwareAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(Engine::Instance()->device.GetAddressOf()))))
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
		if (FAILED(D3D12CreateDevice(warpAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(Engine::Instance()->device.GetAddressOf()))))
		{
			MessageBox(NULL, L"�I������WARP�f�o�C�X�܂ł���DirectX12�ɑΉ����Ă��܂���B", WINDOW_TITLE, MB_OK | MB_ICONERROR);
			return E_FAIL;
		}
	}
}

HRESULT Library::Engine::InitCommandQueue()
{
	//�R�}���h�L���[�̐ݒ�
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

	//�R�}���h�L���[���쐬
	if (FAILED(Engine::Instance()->device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(Engine::Instance()->commandQueue.GetAddressOf()))))
	{
		MessageBox(NULL, L"�R�}���h�L���[���쐬�ł��܂���ł����B", WINDOW_TITLE, MB_OK | MB_ICONERROR);
		return E_FAIL;
	}
}

HRESULT Library::Engine::InitFance()
{
	// �t�F���X���쐬
	if (FAILED(Engine::Instance()->device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(Engine::Instance()->fence.GetAddressOf()))))
	{
		MessageBox(NULL, L"�t�F���X���쐬�ł��܂���ł����B", WINDOW_TITLE, MB_OK | MB_ICONERROR);
		return E_FAIL;
	}
	// �t�F���X�Ƃ́A�����I�u�W�F�N�g�Ƃ��ă��\�[�X��GPU�ɃA�b�v���[�h�����܂őҋ@������́B

	// �t�F���X�l��1�ɐݒ�
	Engine::Instance()->fenceValue = 1;

	// �t���[�������Ɏg�p����C�x���g�n���h�����쐬
	Engine::Instance()->fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	if (Engine::Instance()->fenceEvent == nullptr)
	{
		MessageBox(NULL, L"�t�F���X�C�x���g�n���h�����쐬�ł��܂���ł����B", WINDOW_TITLE, MB_OK | MB_ICONERROR);
		return E_FAIL;
	}
	// �҂ۂ�Windows��Event�𗘗p
}

HRESULT Library::Engine::InitRtvHeapDesc()
{
	//�����_�[�^�[�Q�b�g�r���[�p�̃f�B�X�N���v�^�[�q�[�v�̐ݒ�
	rtvHeapDesc.NumDescriptors = Engine::Instance()->frameCount;
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	// �f�B�X�N���v�^�Ƃ́AGPU�ƃ��\�[�X�̋��n�����s�������̂��́B
	// DX11�܂ł͉B������Ă����B
	// �f�B�X�N���v�^�ɂ�3�̊T�O�����݂���B
	// Descriptor�F�e�N�X�`���Ȃǂ̃��\�[�X��GPU�ƕR�Â���B
	// DescriptorHeap�FDescriptorHeap����Descriptor���쐬����B�Ǘ��ł���Descriptor�̎�ނ␔�͎��O�Ɏw��B
	// DescriptorTable�FGPU��Ŏg�p����Descriptor�̐���z�u�𐧌䂷��BDescriptorTable��RootSignature�Őݒ肷��B

	// �f�B�X�N���v�^�q�[�v���쐬
	if (FAILED(Engine::Instance()->device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(Engine::Instance()->rtvHeap.GetAddressOf()))))
	{
		MessageBox(NULL, L"�f�B�X�N���v�^�q�[�v���쐬�ł��܂���ł����B", WINDOW_TITLE, MB_OK | MB_ICONERROR);
		return E_FAIL;
	}

	//�f�B�X�N���v�^�̃T�C�Y���擾
	Engine::Instance()->rtvDescriptorSize = Engine::Instance()->device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

}

void Library::Engine::End()
{
	OnDestroy();
}