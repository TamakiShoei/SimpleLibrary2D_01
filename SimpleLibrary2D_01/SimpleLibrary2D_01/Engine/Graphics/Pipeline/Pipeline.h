#ifndef	PIPELINE_SIGNATURE_H
#define PIPELINE_SIGNATURE_H

#include "../../CommonDifinition.h"

class Pipeline
{
public:
	Pipeline();
	~Pipeline();

	bool Initialize(ComPtr<ID3D12Device> device, ComPtr<ID3D12RootSignature> rootSignature);
	void Finalize();

	ID3D12PipelineState* Get()
	{
		return this->instance.Get();
	}

private:
	ComPtr<ID3D12PipelineState> instance;
};

#endif