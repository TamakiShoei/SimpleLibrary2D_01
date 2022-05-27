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

//引数がtrueかを判定
#define B_SUCCEEDED(val)   (((bool)(val)) >= 0)
//引数がfalseかを判定
#define B_FAILED(val)      (((bool)(val)) < 0)

#define WINDOW_CLASS	L"初期化(ブルー画面)"
#define WINDOW_TITLE	WINDOW_CLASS
//#define WINDOW_WIDTH	750
//#define WINDOW_HEIGHT	500

//usingディレクティブ
using Microsoft::WRL::ComPtr;
#endif