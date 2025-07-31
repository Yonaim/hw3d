#include <windows.h>
#include "Window.h"
#include "WindowsMessageMap.h"
#include <sstream>

std::wstring to_wstring(const char *src)
{
	int          len = MultiByteToWideChar(CP_UTF8, 0, src, -1, nullptr, 0);
	std::wstring wstr(len, 0);
	MultiByteToWideChar(CP_UTF8, 0, src, -1, &wstr[0], len);
	return wstr;
}
  
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static WindowsMessageMap mm;
	OutputDebugStringA(mm(msg, lParam, wParam).c_str());

	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(69);
		break;
	case WM_KEYDOWN:
		if (wParam == 'F')
			SetWindowText(hWnd, L"Yeeeeaaaah");
		break;
	case WM_KEYUP:
		if (wParam == 'F')
			SetWindowText(hWnd, L"NOOOOOOOOO");
		break;
	case WM_CHAR:
	{
		static std::string title;
		title.push_back((char)wParam);
		SetWindowTextA(hWnd, title.c_str());
	}
	break;
	case WM_LBUTTONDOWN:
	{
		const POINTS       pt = MAKEPOINTS(lParam);
		std::ostringstream oss;
		oss << "(" << pt.x << "," << pt.y << ")";
		SetWindowTextA(hWnd, oss.str().c_str());
	}
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

int CALLBACK WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow )
{
	try
	{
		Window wnd(800, 300, L"Donkey Fart Box");

		MSG  msg;
		BOOL gResult;
		while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (gResult == -1)
			return -1;
		else
			return msg.wParam;
	}
	catch (const ChiliException &e)
	{
		MessageBoxA(nullptr, e.what(), e.GetType(),
				   MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const std::exception &e)
	{
		MessageBoxA(nullptr, e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...)
	{
		MessageBoxA(nullptr, "No details available", "Unknown Exception",
				   MB_OK | MB_ICONEXCLAMATION);
	}
}
