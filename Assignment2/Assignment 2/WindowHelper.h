#pragma once

#include <Windows.h>

struct RenderWindow
{
	HWND handle;
};

RenderWindow CreateRenderWindow(HINSTANCE instance, UINT width, UINT height,
	LPCWSTR className, WNDPROC windowProcedure, LPCWSTR windowName, int cmdShow);
