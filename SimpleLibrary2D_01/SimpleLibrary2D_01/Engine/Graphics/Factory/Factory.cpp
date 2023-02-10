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
	//�t�@�N�g�����쐬
	if (FAILED(CreateDXGIFactory2(factory_flags, IID_PPV_ARGS(factory.GetAddressOf()))))
	{
		MessageBox(NULL, L"�t�@�N�g�����쐬�ł��܂���ł����B", WINDOW_TITLE, MB_OK | MB_ICONERROR);
		return false;
	}
	//Alt+Enter�ɂ��S��ʑJ�ڂ��ł��Ȃ��悤�ɂ���
	if (FAILED(factory->MakeWindowAssociation(FindWindow(WINDOW_TITLE, nullptr), DXGI_MWA_NO_ALT_ENTER)))
	{
		MessageBox(NULL, L"��ʂ̐ݒ肪�ł��܂���B", WINDOW_TITLE, MB_OK | MB_ICONERROR);
		return false;
	}


	return true;

}

void Factory::Finalize()
{
	factory->Release();
}