#include "Input.h"

Input::Input()
{
	for (int i = 0; i < MAX_KEY; i++)
	{
		keyStates[i].IsCurrentPushed = false;
		keyStates[i].IsPrevPushed = false;
	}
}

Input::~Input()
{
	
}

bool Input::Initialize()
{
	//IDirectInput8�̍쐬
	HRESULT result;
	result = DirectInput8Create(
		GetModuleHandle(NULL),
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(LPVOID*)&directInput,
		NULL);
	if (FAILED(result))
	{
		return false;
	}

	//IDirectInputDevice8�̎擾
	result = directInput->CreateDevice(
		GUID_SysKeyboard,
		&keyboard,
		NULL);
	if (FAILED(result))
	{
		return false;
	}

	//���̓f�[�^�`���̃Z�b�g
	result = keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(result))
	{
		keyboard->Release();
		directInput->Release();
		return false;
	}

	//�r������̃Z�b�g
	result = keyboard->SetCooperativeLevel(
		FindWindow(WINDOW_TITLE, NULL),
		DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	if (FAILED(result))
	{
		keyboard->Release();
		directInput->Release();
		return false;
	}

	return true;
}

void Input::Update()
{
	HRESULT result;
	BYTE buffer[MAX_KEY];
	ZeroMemory(buffer, sizeof(buffer));

	if (keyboard == nullptr)
	{
		return;
	}
	result = keyboard->GetDeviceState(sizeof(buffer), buffer);
	if (FAILED(result))
	{
		keyboard->Acquire();
		keyboard->GetDeviceState(sizeof(buffer), buffer);
	}

	for (int i = 0; i < MAX_KEY; i++)
	{
		keyStates[i].IsPrevPushed = keyStates[i].IsCurrentPushed;
		if ((BYTE)buffer[i] & 0x80)
		{
			keyStates[i].IsCurrentPushed = true;
		}
		else
		{
			keyStates[i].IsCurrentPushed = false;
		}
	}
}

void Input::Finalize()
{
	keyboard->Release();
	directInput->Release();
}

bool Input::IsPushed(int key)
{
	if (key < 0 || key > MAX_KEY)
	{
		return false;
	}

	if (keyStates[key].IsPrevPushed == false &&
		keyStates[key].IsCurrentPushed == true)
	{
		return true;
	}
	return false;
}

bool Input::IsHeld(int key)
{
	if (key < 0 || key > MAX_KEY)
	{
		return false;
	}

	if (keyStates[key].IsPrevPushed == true &&
		keyStates[key].IsCurrentPushed == true)
	{
		return true;
	}
	return false;
}

bool Input::IsReleased(int key)
{
	if (key < 0 || key > MAX_KEY)
	{
		return false;
	}

	if (keyStates[key].IsPrevPushed == true &&
		keyStates[key].IsCurrentPushed == false)
	{
		return true;
	}
	return false;
}
