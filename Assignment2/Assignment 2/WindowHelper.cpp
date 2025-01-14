#include "WindowHelper.h"

#include <string>
#include <stdexcept>

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// sort through and find what code to run for the message given
	switch (message)
	{
		// this message is read when the window is closed
	case WM_DESTROY:
	{
		// close the application entirely
		PostQuitMessage(0);
		return 0;
	}
	default:
		break;
	}

	// Handle any messages the switch statement didn't by using default methodology
	return DefWindowProc(hWnd, message, wParam, lParam);
}

RenderWindow CreateRenderWindow(HINSTANCE instance, UINT width, UINT height, 
	LPCWSTR className, WNDPROC windowProcedure, LPCWSTR windowName, int cmdShow)
{
	RenderWindow toReturn;

	WNDCLASS wc = { };
	wc.lpfnWndProc = (windowProcedure == nullptr) ? WindowProc : windowProcedure;
	wc.hInstance = instance;
	wc.lpszClassName = className;
	RegisterClass(&wc);

	RECT windowRect = { 0, 0, static_cast<LONG>(width), static_cast<LONG>(height) };
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

	toReturn.handle = CreateWindowEx(0, className, windowName, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, windowRect.right - windowRect.left, 
		windowRect.bottom - windowRect.top, nullptr, nullptr, instance, nullptr);

	if (toReturn.handle == nullptr)
	{
		std::string errorMessage = "Could not create window, GetLastError(): ";
		errorMessage += std::to_string(GetLastError());
		throw std::runtime_error(errorMessage.c_str());
	}

	ShowWindow(toReturn.handle, cmdShow);

	return toReturn;
}
