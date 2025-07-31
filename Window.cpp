﻿#include "Window.h"

// =============================================================================
//                            Window::WindowClass
// =============================================================================

Window::WindowClass Window::WindowClass::wndClass;

Window::WindowClass::WindowClass() noexcept : hInst(GetModuleHandle(nullptr))
{
	// register window class
	WNDCLASSEXW wc   = {0};
	wc.cbSize        = sizeof(wc);
	wc.style         = CS_OWNDC;
	wc.lpfnWndProc   = HandleMsgSetup;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = GetInstance();
	wc.hIcon         = nullptr;
	wc.hCursor       = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName  = nullptr;
	wc.lpszClassName = GetName();
	wc.hIconSm       = nullptr;
	RegisterClassExW(&wc);
}

Window::WindowClass::~WindowClass()
{
	UnregisterClassW(wndClassName, GetInstance());
}

const wchar_t *Window::WindowClass::GetName() noexcept
{
	return wndClassName;
}

HINSTANCE Window::WindowClass::GetInstance() noexcept
{
	return wndClass.hInst;
}

// =============================================================================
//                            Window::Exception
// =============================================================================
#include <sstream>
Window::Exception::Exception(int line, const char *file, HRESULT hr) noexcept
	: ChiliException(line, file),
	  hr(hr)
{
}

const char *Window::Exception::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code]" << GetErrorCode() << std::endl
		<< "[Decription]" << GetErrorCode() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char *Window::Exception::GetType() const noexcept
{
	return "Chili Window Exception";
}

std::string Window::Exception::TranslateErrorCode(HRESULT hr)
{
	char *pMsgBuf = nullptr;
	DWORD nMsgLen = FormatMessage(
		// API 측에서 직접 메모리 할당하도록 지시
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM
			| FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPWSTR>(&pMsgBuf), 0, nullptr);
	if (nMsgLen == 0)
		return "Unidentified error code";
	std::string errorString = pMsgBuf;
	LocalFree(pMsgBuf); // API가 할당한 힙 영역 해제하는 함수 호출
	return errorString;
}

HRESULT Window::Exception::GetErrorCode() const noexcept
{
	return hr;
}

std::string Window::Exception::GetErrorString() const noexcept
{
	return TranslateErrorCode(hr);
}

// =============================================================================
//                                  Window
// =============================================================================

Window::Window(int width, int height, const wchar_t *name)
{
	// caculate window size based on desired client region size
	RECT wr;
	wr.left   = 100; // client region coordinates of screen space
	wr.top    = 100;
	wr.right  = width + wr.left;
	wr.bottom = height + wr.top;
	// adjust window size to account for title bar, menu bar, etc.
	AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);

	throw CHWND_EXCEPT(ERROR_ARENA_TRASHED);

	// create window & get hWnd
	hWnd = CreateWindowW(
		WindowClass::GetName(), name, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
		nullptr, nullptr, WindowClass::GetInstance(), this);
	ShowWindow(hWnd, SW_SHOWDEFAULT);
}

LRESULT CALLBACK Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam,
										LPARAM lParam)
{
	if (msg == WM_NCCREATE) // non-client region create (�� ó��)
	{
		const CREATESTRUCTW *const pCreate
			= reinterpret_cast<CREATESTRUCTW *>(lParam);
		Window *const pWnd = static_cast<Window *>(pCreate->lpCreateParams);

		// Store pointer to window instance in user data
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));

		// Subclass window procedure to call window instance handler
		// thunk lpfnWndProc will call this
		SetWindowLongPtr(hWnd, GWLP_WNDPROC,
						 reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));
		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	}
	else
		return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam,
										LPARAM lParam)
{
	// retrieve ptr to window instance
	Window *const pWnd
		= reinterpret_cast<Window *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	// forward message to window instance handler
	return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam,
								   LPARAM lParam)
{
	// Store pointer to window instance in user data
	// Window* const pWnd = reinterpret_cast< Window* >( GetWindowLongPtr (hWnd
	// , GWLP_USERDATA) ); return pWnd->HandleMsg (hWnd , msg , wParam ,
	// lParam);

	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		return (0);
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

Window::~Window()
{
}