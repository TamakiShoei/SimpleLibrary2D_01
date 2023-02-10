#ifndef SWAP_CHAIN
#define	SWAP_CHAIN

#include "../../CommonDifinition.h"

class SwapChain
{
public:
	SwapChain();
	~SwapChain();

	bool Initialize(ComPtr<IDXGIFactory4> factory, ComPtr<ID3D12CommandQueue> commandQueue);
	void UpdateFrameIndex();
	void Finalize();

	inline IDXGISwapChain4* Get()
	{
		return this->swapChain.Get();
	}

	inline UINT GetFrameIndex()
	{
		return this->frameIndex;
	}

private:
	ComPtr<IDXGISwapChain4> swapChain;
	UINT frameIndex;
};

#endif