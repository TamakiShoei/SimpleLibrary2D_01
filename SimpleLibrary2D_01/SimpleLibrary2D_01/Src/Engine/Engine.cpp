#include "Library.h"

HRESULT Library::Engine::OnInit()
{
	if (FAILED(GetGraphics()->Initialize()))
	{
		return E_FAIL;
	}
	return S_OK;
}

VOID Library::Render::ClearScreen()
{
	// �R�}���h�A���P�[�^�����Z�b�g
	Library::Engine::Instance()->GetGraphics()->commandAllocator->Reset();

	// �R�}���h���X�g�����Z�b�g
	Engine::Instance()->GetGraphics()->commandList->Reset(Engine::Instance()->GetGraphics()->commandAllocator.Get(), Engine::Instance()->GetGraphics()->pipelineState.Get());

	// �o�b�N�o�b�t�@�������_�[�^�[�Q�b�g�Ƃ��Ďg�p
	auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(Engine::Instance()->GetGraphics()->renderTargets[Engine::Instance()->GetGraphics()->frameIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	Engine::Instance()->GetGraphics()->commandList->ResourceBarrier(1, &barrier);
	// ���\�[�X�o���A�Ƃ́AGPU���ň������\�[�X�̏󋵂𓯊�������@�\�B
	// �}���`�X���b�h��O��Ƃ��������Ȃ̂ŁAGPU���̓���������̃A�N�Z�X�������ɍs���邱�Ƃ�z�肵���@�\���Ƃ������ƁB

	// �����_�[�^�[�Q�b�g�r���[�̃n���h�����쐬
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(Engine::Instance()->GetGraphics()->rtvHeap->GetCPUDescriptorHandleForHeapStart(), Engine::Instance()->GetGraphics()->frameIndex, Engine::Instance()->GetGraphics()->rtvDescriptorSize);

	// �o�b�N�o�b�t�@�ɕ`��(�R�}���h���L�^)
	const FLOAT	clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };		// ���ۂ��F
	Engine::Instance()->GetGraphics()->commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
}

VOID Library::Render::ScreenFlip()
{
	// �o�b�N�o�b�t�@��\��
	auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(Engine::Instance()->GetGraphics()->renderTargets[Engine::Instance()->GetGraphics()->frameIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	Engine::Instance()->GetGraphics()->commandList->ResourceBarrier(1, &barrier);

	// �R�}���h���X�g���N���[�Y
	Engine::Instance()->GetGraphics()->commandList->Close();

	// �R�}���h���X�g�����s
	ID3D12CommandList* ppCommandLists[] = { Engine::Instance()->GetGraphics()->commandList.Get() };
	Engine::Instance()->GetGraphics()->commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

	// �t���[�����ŏI�o��
	Engine::Instance()->GetGraphics()->swapChain->Present(1, 0);

	// �t���[���㏈��
	Library::Engine::Instance()->WaitForPreviousFrame();
}

VOID Library::Engine::WaitForPreviousFrame()
{
	const UINT64 fence = Engine::Instance()->GetGraphics()->fenceValue;
	Engine::Instance()->GetGraphics()->commandQueue->Signal(Engine::Instance()->GetGraphics()->fence.Get(), fence);
	Engine::Instance()->GetGraphics()->fenceValue++;

	// �O�̃t���[�����I������܂őҋ@
	if (Engine::Instance()->GetGraphics()->fence->GetCompletedValue() < fence) {
		Engine::Instance()->GetGraphics()->fence->SetEventOnCompletion(fence, Engine::Instance()->GetGraphics()->fenceEvent);
		WaitForSingleObject(Engine::Instance()->GetGraphics()->fenceEvent, INFINITE);
	}

	// �o�b�N�o�b�t�@�̃C���f�b�N�X���i�[
	Engine::Instance()->GetGraphics()->frameIndex = Engine::Instance()->GetGraphics()->swapChain->GetCurrentBackBufferIndex();
}

VOID Library::Engine::OnDestroy()
{
	WaitForPreviousFrame();
	/*CloseHandle(GetGraphics()->fenceEvent);*/
}

void Library::Engine::End()
{
	OnDestroy();
}