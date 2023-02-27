#include "CommandQueue.h"

CommandQueue::CommandQueue() : instance(nullptr)
{

}

CommandQueue::~CommandQueue()
{
	if (instance != nullptr)
	{
		instance->Release();
	}
}

bool CommandQueue::Initialize(ComPtr<ID3D12Device> device)
{
	//�R�}���h�L���[�̐ݒ�
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc = {};
	commandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	commandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

	//�R�}���h�L���[���쐬
	if (FAILED(device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(instance.GetAddressOf()))))
	{
		MessageBox(NULL, L"�R�}���h�L���[���쐬�ł��܂���ł����B", WINDOW_TITLE, MB_OK | MB_ICONERROR);
		return false;
	}

	return true;
}

void CommandQueue::Finalize()
{
	instance->Release();
}