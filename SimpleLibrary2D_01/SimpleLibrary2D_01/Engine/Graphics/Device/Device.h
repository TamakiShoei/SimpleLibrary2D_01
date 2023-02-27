#ifndef	DEVICE_H
#define DEVICE_H

#include "../../CommonDifinition.h"

class Device
{
public:
	Device();
	~Device();

	bool Initialize(ComPtr<IDXGIFactory4> factory);
	void Finalize();

	inline ID3D12Device* Get()
	{
		return this->instance.Get();
	}

private:
	ComPtr<ID3D12Device> instance;
	std::vector<D3D_FEATURE_LEVEL> levels;
};

#endif	