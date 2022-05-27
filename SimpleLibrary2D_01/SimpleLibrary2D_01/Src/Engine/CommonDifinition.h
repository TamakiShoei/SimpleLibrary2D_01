#ifndef COMMON_DIFINITION_H 
#define	COMMON_DIFINITION_H

#include <windows.h>
#include <wrl.h>

//DirectX12のコードセット
#include <d3d12.h>
#include <dxgi1_4.h>
#include "d3dx12.h"

//ライブラリ
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

#define B_SUCCEEDED(bl)   (((bool)(bl)) >= 0)
#define B_FAILED(bl)      (((bool)(bl)) < 0)

#define WINDOW_CLASS	L"初期化(ブルー画面)"
#define WINDOW_TITLE	WINDOW_CLASS
//#define WINDOW_WIDTH	750
//#define WINDOW_HEIGHT	500

//usingディレクティブ
using Microsoft::WRL::ComPtr;
#endif