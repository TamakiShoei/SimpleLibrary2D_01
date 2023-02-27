#include "SwapChain.h"

SwapChain::SwapChain() : instance(nullptr), frameIndex(0)
{

}

SwapChain::~SwapChain()
{
	if (instance != nullptr)
	{
		instance->Release();
	}
}

bool SwapChain::Initialize(ComPtr<IDXGIFactory4> factory, ComPtr<ID3D12CommandQueue> commandQueue)
{
	RECT rect;
	GetClientRect(FindWindow(WINDOW_TITLE, nullptr), &rect);

	//フロントバッファとバックバッファの二つ
	const UINT frameCount = 2;

	//スワップチェインの設定
	DXGI_SWAP_CHAIN_DESC1 desc = {};
	desc.BufferCount = frameCount;
	desc.Width = rect.right;
	desc.Height = rect.bottom;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	desc.SampleDesc.Count = 1;
	//スワップチェインとは、レンダリング結果を出力するためのオブジェクト
	//紐づいたビデオアダプタやウィンドウに対してレンダリング結果を出力する

	//スワップチェインを作成
	ComPtr<IDXGISwapChain1>	tmpSwapChain;
	if (FAILED(factory->CreateSwapChainForHwnd(commandQueue.Get(), FindWindow(WINDOW_TITLE, nullptr), &desc, nullptr, nullptr, tmpSwapChain.GetAddressOf())))
	{
		MessageBox(NULL, L"スワップチェインを作成できませんでした。", WINDOW_TITLE, MB_OK | MB_ICONERROR);
		return false;
	}

	//スワップチェインをキャスト
	tmpSwapChain.As(&instance);

	//バックバッファのインデックスを格納
	frameIndex = instance->GetCurrentBackBufferIndex();

	return true;
}

void SwapChain::UpdateFrameIndex()
{
	frameIndex = instance->GetCurrentBackBufferIndex();
}

void SwapChain::Finalize()
{
	instance->Release();
}