#ifndef	ROOT_SIGNATURE_H
#define ROOT_SIGNATURE_H

#include "../../CommonDifinition.h"

class RootSignature
{
public:
	RootSignature();
	~RootSignature();

	bool Initialize(ComPtr<ID3D12Device> device);
	void Finalize();

	ID3D12RootSignature* Get()
	{
		return this->rootSignature.Get();
	}

private:
	ComPtr<ID3D12RootSignature> rootSignature;
};

#endif