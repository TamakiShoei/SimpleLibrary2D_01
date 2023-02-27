#ifndef COMMAND_ALLOCATOR_H
#define	COMMAND_ALLOCATOR_H

#include "../../CommonDifinition.h"

class CommandAllocator
{
public:
	CommandAllocator();
	~CommandAllocator();

	bool Initialize(ComPtr<ID3D12Device> device);
	void Finalize();

	inline ID3D12CommandAllocator* Get()
	{
		return this->instance.Get();
	}

private:
	ComPtr<ID3D12CommandAllocator> instance;
};

#endif