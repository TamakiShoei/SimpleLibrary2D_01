#include "CommandList.h"

CommandList::CommandList() : instance(nullptr)
{

}

CommandList::~CommandList()
{
	if (instance != nullptr)
	{
		instance->Release();
	}
}

bool CommandList::Initialize(ComPtr<ID3D12Device> device, ComPtr<ID3D12CommandAllocator> commandAllocator)
{
	// コマンドリストを作成
	if (FAILED(device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator.Get(), nullptr, IID_PPV_ARGS(instance.GetAddressOf()))))
	{
		MessageBox(NULL, L"コマンドリストを作成できませんでした。", WINDOW_TITLE, MB_OK | MB_ICONERROR);
		return false;
	}
	// コマンドリストは記録状態で作成されるが、今回は初期化内でそこに何も入れないのですぐに閉じる。
	instance.Get()->Close();
	return true;
}

void CommandList::Finalize()
{
	instance->Release();
}