#include "CommandList.h"

CommandList::CommandList() : commandList(nullptr)
{

}

CommandList::~CommandList()
{
	if (commandList != nullptr)
	{
		commandList->Release();
	}
}

bool CommandList::Initialize(ComPtr<ID3D12Device> device, ComPtr<ID3D12CommandAllocator> commandAllocator)
{
	// �R�}���h���X�g���쐬
	if (FAILED(device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator.Get(), nullptr, IID_PPV_ARGS(commandList.GetAddressOf()))))
	{
		MessageBox(NULL, L"�R�}���h���X�g���쐬�ł��܂���ł����B", WINDOW_TITLE, MB_OK | MB_ICONERROR);
		return false;
	}
	// �R�}���h���X�g�͋L�^��Ԃō쐬����邪�A����͏��������ł����ɉ�������Ȃ��̂ł����ɕ���B
	commandList.Get()->Close();
	return true;
}

void CommandList::Finalize()
{
	commandList->Release();
}