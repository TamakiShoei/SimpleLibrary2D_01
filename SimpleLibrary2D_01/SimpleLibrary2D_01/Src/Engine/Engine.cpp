#include "Engine.h"

Engine* Engine::instance = nullptr;

bool Engine::Initialize()
{
	if (instance == nullptr)
	{
		instance = new Engine();
	}

	if (B_FAILED(instance->window.Initialize()))
	{
		return false;
	}
	if (B_FAILED(instance->graphics.Initialize()))
	{
		return false;
	}
	return true;
}

void Engine::Update()
{
	instance->input.Update();
	instance->window.Update();
}

void Engine::Finalize()
{
	UnregisterClass(WINDOW_CLASS_NAME, GetModuleHandle(nullptr));
	instance->graphics.WaitForPreviousFrame();
}

//Windowクラス関係の関数
bool Engine::IsClosedWindow()
{
	return instance->window.IsClosed();
}

void Engine::SetWindowSize(int width_size, int height_size)
{
	if (instance == nullptr)
	{
		instance = new Engine();
	}

	return instance->window.SetWindowSize(width_size, height_size);
}

bool Engine::IsPushedKey(int key)
{
	if (instance->input.IsPushed(key) == true)
	{
		return true;
	}

	return false;
}

bool Engine::IsHeldKey(int key)
{
	if (instance->input.IsHeld(key) == true)
	{
		return true;
	}

	return false;
}

bool Engine::IsReleasedKey(int key)
{
	if (instance->input.IsReleased(key) == true)
	{
		return true;
	}

	return false;
}

//Graphicsクラス関係の関数
void Engine::ClearScreen()
{
	instance->graphics.ClearScreen();
}

void Engine::ScreenFlip()
{
	instance->graphics.ScreenFlip();
}

void Engine::DrawTriangle(VECTOR lower_left, VECTOR upper_left, VECTOR lower_right)
{
	instance->graphics.DrawTriangle(lower_left, upper_left, lower_right);
}

void Engine::DrawRect(VECTOR lower_left, VECTOR upper_left, VECTOR upper_right, VECTOR lower_right)
{
	instance->graphics.DrawRect(lower_left, upper_left, upper_right, lower_right);
}

void Engine::DrawTexture(float pos_x, float pos_y, const char* file_path)
{
	instance->graphics.DrawTexture(pos_x, pos_y, file_path);
}