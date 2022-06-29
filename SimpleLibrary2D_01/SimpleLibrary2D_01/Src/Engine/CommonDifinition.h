#ifndef COMMON_DIFINITION_H 
#define	COMMON_DIFINITION_H

#include <windows.h>
#include <wrl.h>

//DirectX12のコードセット
#include <d3d12.h>
#include <dxgi1_6.h>
#include "d3dx12.h"

//ライブラリ
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

//引数がtrueかを判定
#define B_SUCCEEDED(val)   (((bool)(val)) >= 0)
//引数がfalseかを判定
#define B_FAILED(val)      (((bool)(val)) < 0)

struct VECTOR
{
	float x;
	float y;
	float z;
};

#define WINDOW_CLASS	L"DirectX12 自作ライブラリ"
#define WINDOW_TITLE	WINDOW_CLASS

//usingディレクティブ
using Microsoft::WRL::ComPtr;
#endif