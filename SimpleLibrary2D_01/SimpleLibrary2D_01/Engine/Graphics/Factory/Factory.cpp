#include "Factory.h"

Factory::Factory() : factory(nullptr)
{

}

Factory::~Factory()
{
	if (factory != nullptr)
	{
		factory->Release();
	}
}

bool Factory::Initialize(UINT factory_flags)
{
	//ファクトリを作成
	if (FAILED(CreateDXGIFactory2(factory_flags, IID_PPV_ARGS(factory.GetAddressOf()))))
	{
		MessageBox(NULL, L"ファクトリを作成できませんでした。", WINDOW_TITLE, MB_OK | MB_ICONERROR);
		return false;
	}
	//Alt+Enterによる全画面遷移をできないようにする
	if (FAILED(factory->MakeWindowAssociation(FindWindow(WINDOW_TITLE, nullptr), DXGI_MWA_NO_ALT_ENTER)))
	{
		MessageBox(NULL, L"画面の設定ができません。", WINDOW_TITLE, MB_OK | MB_ICONERROR);
		return false;
	}


	return true;

}

void Factory::Finalize()
{
	factory->Release();
}