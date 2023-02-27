#include "SwapChain.h"

SwapChain::SwapChain() : instance(nullptr), frameIndex(0)
{

}

SwapChain::~SwapChain()
{
	if (instance != nullptr)
	{
		instance->Release();
	}
}

bool SwapChain::Initialize(ComPtr<IDXGIFactory4> factory, ComPtr<ID3D12CommandQueue> commandQueue)
{
	RECT rect;
	GetClientRect(FindWindow(WINDOW_TITLE, nullptr), &rect);

	//�t�����g�o�b�t�@�ƃo�b�N�o�b�t�@�̓��
	const UINT frameCount = 2;

	//�X���b�v�`�F�C���̐ݒ�
	DXGI_SWAP_CHAIN_DESC1 desc = {};
	desc.BufferCount = frameCount;
	desc.Width = rect.right;
	desc.Height = rect.bottom;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	desc.SampleDesc.Count = 1;
	//�X���b�v�`�F�C���Ƃ́A�����_�����O���ʂ��o�͂��邽�߂̃I�u�W�F�N�g
	//�R�Â����r�f�I�A�_�v�^��E�B���h�E�ɑ΂��ă����_�����O���ʂ��o�͂���

	//�X���b�v�`�F�C�����쐬
	ComPtr<IDXGISwapChain1>	tmpSwapChain;
	if (FAILED(factory->CreateSwapChainForHwnd(commandQueue.Get(), FindWindow(WINDOW_TITLE, nullptr), &desc, nullptr, nullptr, tmpSwapChain.GetAddressOf())))
	{
		MessageBox(NULL, L"�X���b�v�`�F�C�����쐬�ł��܂���ł����B", WINDOW_TITLE, MB_OK | MB_ICONERROR);
		return false;
	}

	//�X���b�v�`�F�C�����L���X�g
	tmpSwapChain.As(&instance);

	//�o�b�N�o�b�t�@�̃C���f�b�N�X���i�[
	frameIndex = instance->GetCurrentBackBufferIndex();

	return true;
}

void SwapChain::UpdateFrameIndex()
{
	frameIndex = instance->GetCurrentBackBufferIndex();
}

void SwapChain::Finalize()
{
	instance->Release();
}