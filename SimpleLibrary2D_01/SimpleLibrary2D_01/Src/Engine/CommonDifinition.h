#ifndef COMMON_DIFINITION_H 
#define	COMMON_DIFINITION_H

#include <windows.h>
#include <wrl.h>

//DirectX12�̃R�[�h�Z�b�g
#include <d3d12.h>
#include <dxgi1_4.h>
#include "d3dx12.h"

//���C�u����
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

#define WINDOW_CLASS	L"������(�u���[���)"
#define WINDOW_TITLE	WINDOW_CLASS
#define WINDOW_WIDTH	750
#define WINDOW_HEIGHT	500

//using�f�B���N�e�B�u
using Microsoft::WRL::ComPtr;
#endif