#include "Fence.h"

Fence::Fence() : fence(nullptr), fenceEvent(nullptr), fenceValue(0)
{

}

Fence::~Fence()
{
	if (fence != nullptr)
	{
		fence->Release();
	}
}

bool Fence::Initialize(ComPtr<ID3D12Device> device)
{
	// �t�F���X���쐬
	if (FAILED(device.Get()->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(fence.GetAddressOf()))))
	{
		MessageBox(NULL, L"�t�F���X���쐬�ł��܂���ł����B", WINDOW_TITLE, MB_OK | MB_ICONERROR);
		return false;
	}
	// �t�F���X�Ƃ́A�����I�u�W�F�N�g�Ƃ��ă��\�[�X��GPU�ɃA�b�v���[�h�����܂őҋ@������́B

	// �t�F���X�l��1�ɐݒ�
	fenceValue = 1;

	// �t���[�������Ɏg�p����C�x���g�n���h�����쐬
	fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	if (fenceEvent == nullptr)
	{
		MessageBox(NULL, L"�t�F���X�C�x���g�n���h�����쐬�ł��܂���ł����B", WINDOW_TITLE, MB_OK | MB_ICONERROR);
		return false;
	}
	return true;
}

void Fence::WaitForPreviousFrame(ComPtr<ID3D12CommandQueue> commandQueue)
{
	const UINT64 tmpFence = fenceValue;
	commandQueue.Get()->Signal(fence.Get(), tmpFence);
	fenceValue++;

	// �O�̃t���[�����I������܂őҋ@
	if (fence->GetCompletedValue() < tmpFence) {
		fence->SetEventOnCompletion(tmpFence, fenceEvent);
		WaitForSingleObject(fenceEvent, INFINITE);
	}

}

void Fence::Finalize()
{
	fence->Release();
}