#include "Device.h"

Device::Device() : instance(nullptr)
{
	levels.push_back(D3D_FEATURE_LEVEL_12_1);
	levels.push_back(D3D_FEATURE_LEVEL_12_0);
	levels.push_back(D3D_FEATURE_LEVEL_11_1);
	levels.push_back(D3D_FEATURE_LEVEL_11_0);
}

Device::~Device()
{
	if (instance != nullptr)
	{
		instance.ReleaseAndGetAddressOf();
	}
}

bool Device::Initialize(ComPtr<IDXGIFactory4> factory)
{
	HRESULT hr;
	std::vector<IDXGIAdapter*> adapters;
	IDXGIAdapter* tmpAdapter = nullptr;
	for (int i = 0; factory->EnumAdapters(i, &tmpAdapter) != DXGI_ERROR_NOT_FOUND; i++)
	{
		adapters.push_back(tmpAdapter);
	}
	for (auto adpt : adapters)
	{
		DXGI_ADAPTER_DESC desc = {};
		adpt->GetDesc(&desc);
		std::wstring strDesc = desc.Description;
		if (strDesc.find(L"NVIDIA") != std::string::npos)
		{
			tmpAdapter = adpt;
			break;
		}
	}

	//デバイスの作成
	UINT failedCounter = 0;
	for (auto l : levels)
	{
		if (D3D12CreateDevice(tmpAdapter, l, IID_PPV_ARGS(&instance)) == S_OK)
		{
			break;
		}
		else
		{
			failedCounter++;
		}
	}

	if (failedCounter > levels.size())
	{
		return false;
	}
	return true;
}

void Device::Finalize()
{
	instance.ReleaseAndGetAddressOf();
}