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
	// コマンドリストを作成
	if (FAILED(device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator.Get(), nullptr, IID_PPV_ARGS(commandList.GetAddressOf()))))
	{
		MessageBox(NULL, L"コマンドリストを作成できませんでした。", WINDOW_TITLE, MB_OK | MB_ICONERROR);
		return false;
	}
	// コマンドリストは記録状態で作成されるが、今回は初期化内でそこに何も入れないのですぐに閉じる。
	commandList.Get()->Close();
	return true;
}

void CommandList::Finalize()
{
	commandList->Release();
}