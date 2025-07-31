#include <Windows.h>
#include "WindowsMessageMap.h"
#include <sstream>
#include "Window.h"

LRESULT CALLBACK WndProc (HWND hWnd , UINT msg , WPARAM wParam , LPARAM lParam)
{
	static WindowsMessageMap mm;
	OutputDebugStringA(mm(msg, lParam, wParam).c_str());

	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage (69);
		break;
	case WM_KEYDOWN:
		if (wParam == 'F')
			SetWindowText (hWnd , L"Yeeeeaaaah");
		break;
	case WM_KEYUP:
		if (wParam == 'F')
			SetWindowText (hWnd , L"NOOOOOOOOO");
		break;
	case WM_CHAR:
		{
		static std::string title;
		title.push_back (( char ) wParam);
		SetWindowTextA (hWnd , title.c_str ());
		}
		break;
	case WM_LBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS (lParam);
		std::ostringstream oss;
		oss << "(" << pt.x << "," << pt.y << ")";
		SetWindowTextA (hWnd , oss.str ().c_str ());
	}
	}
	return DefWindowProc (hWnd , msg , wParam , lParam);
}

int CALLBACK WinMain (
	HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	Window wnd (800 , 300 , L"Donkey Fart Box");

	MSG msg;
	BOOL gResult;
	while ((gResult = GetMessage (&msg , nullptr , 0 , 0)) > 0)
	{
		TranslateMessage (&msg);
		DispatchMessage (&msg);
	}

	if (gResult == -1)
		return -1;
	else
		return msg.wParam;
}
