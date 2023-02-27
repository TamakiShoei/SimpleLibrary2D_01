#include "CommandAllocator.h"

CommandAllocator::CommandAllocator() : instance(nullptr)
{

}

CommandAllocator::~CommandAllocator()
{
	if (instance != nullptr)
	{
		instance->Release();
	}
}

bool CommandAllocator::Initialize(ComPtr<ID3D12Device> device)
{
	if (FAILED(device.Get()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(instance.GetAddressOf()))))
	{
		MessageBox(NULL, L"コマンドアロケータを作成できませんでした。", WINDOW_TITLE, MB_OK | MB_ICONERROR);
		return false;
	}
	return true;
}

void CommandAllocator::Finalize()
{
	instance->Release();
}