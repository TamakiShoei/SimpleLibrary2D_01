#ifndef	COMMAND_LIST_H
#define COMMAND_LIST_H

#include "../../CommonDifinition.h"

class CommandList
{
public:
	CommandList();
	~CommandList();

	bool Initialize(ComPtr<ID3D12Device> device, ComPtr<ID3D12CommandAllocator> commandAllocator);
	void Finalize();

	inline ID3D12GraphicsCommandList* Get()
	{
		return this->commandList.Get();
	}

private:
	ComPtr<ID3D12GraphicsCommandList> commandList;
};

#endif