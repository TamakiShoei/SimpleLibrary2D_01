#include "Fence.h"

Fence::Fence() : instance(nullptr), fenceEvent(nullptr), fenceValue(0)
{

}

Fence::~Fence()
{
	if (instance != nullptr)
	{
		instance->Release();
	}
}

bool Fence::Initialize(ComPtr<ID3D12Device> device)
{
	// �t�F���X���쐬
	if (FAILED(device.Get()->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(instance.GetAddressOf()))))
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

void Fence::WaitForPreviousFrame(ComPtr<ID3D12CommandQueue> command_queue)
{
	const UINT64 tmpFence = fenceValue;
	command_queue.Get()->Signal(instance.Get(), tmpFence);
	fenceValue++;

	// �O�̃t���[�����I������܂őҋ@
	if (instance->GetCompletedValue() < tmpFence) {
		instance->SetEventOnCompletion(tmpFence, fenceEvent);
		WaitForSingleObject(fenceEvent, INFINITE);
	}

}

void Fence::Finalize()
{
	instance->Release();
}