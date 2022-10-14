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

void Input::Update()
{
	BYTE buffer[256];
	
	GetKeyboardState(buffer);

	for (int i = 0; i < MAX_KEY; ++i)
	{
		keyStates[i].IsPrevPushed = keyStates[i].IsCurrentPushed;

		if (buffer[i] & 0x80)
		{
			keyStates[i].IsCurrentPushed = true;
		}
		else
		{
			keyStates[i].IsCurrentPushed = false;
		}
	}
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
