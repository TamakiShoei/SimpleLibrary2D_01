#include "CommandAllocator.h"

CommandAllocator::CommandAllocator() : commandAllocator(nullptr)
{

}

CommandAllocator::~CommandAllocator()
{
	if (commandAllocator != nullptr)
	{
		commandAllocator->Release();
	}
}

bool CommandAllocator::Initialize(ComPtr<ID3D12Device> device)
{
	if (FAILED(device.Get()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(commandAllocator.GetAddressOf()))))
	{
		MessageBox(NULL, L"コマンドアロケータを作成できませんでした。", WINDOW_TITLE, MB_OK | MB_ICONERROR);
		return false;
	}
	return true;
}

void CommandAllocator::Finalize()
{
	commandAllocator->Release();
}