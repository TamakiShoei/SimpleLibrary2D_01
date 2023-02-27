#include "Factory.h"

Factory::Factory() : instance(nullptr)
{

}

Factory::~Factory()
{
	if (instance != nullptr)
	{
		instance->Release();
	}
}

bool Factory::Initialize(UINT factory_flags)
{
	//ファクトリを作成
	if (FAILED(CreateDXGIFactory2(factory_flags, IID_PPV_ARGS(instance.GetAddressOf()))))
	{
		MessageBox(NULL, L"ファクトリを作成できませんでした。", WINDOW_TITLE, MB_OK | MB_ICONERROR);
		return false;
	}
	//Alt+Enterによる全画面遷移をできないようにする
	if (FAILED(instance->MakeWindowAssociation(FindWindow(WINDOW_TITLE, nullptr), DXGI_MWA_NO_ALT_ENTER)))
	{
		MessageBox(NULL, L"画面の設定ができません。", WINDOW_TITLE, MB_OK | MB_ICONERROR);
		return false;
	}


	return true;

}

void Factory::Finalize()
{
	instance->Release();
}