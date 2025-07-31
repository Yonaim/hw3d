#pragma once
#include "ChiliWin.h"

class Window
{
private:
	// singleton manages registration/cleanup of window class
	class WindowClass
	{
	public:
		static const wchar_t* GetName () noexcept;
		static HINSTANCE GetInstance () noexcept;
	private:
		WindowClass () noexcept; // 외부에서는 생성 불가
		~WindowClass ();
		WindowClass (const WindowClass&) = delete; // Do not use copy
		WindowClass& operator=(const WindowClass&) = delete; // Do not use copy
		static constexpr const wchar_t* wndClassName = L"Chili Direct3D Engine Window"; // 컴파일 시간 상수
		
		static WindowClass wndClass; // WindowClass 클래스 내에 WindowClass 객체가 독립적으로 존재 (singleton)
		HINSTANCE hInst;
	};
public:
	Window (int width , int height , const wchar_t* name) noexcept;
	~Window ();
	Window (const Window&) = delete;
	Window& operator=(const Window&) = delete;
private:
	static LRESULT CALLBACK HandleMsgSetup (HWND hWnd , UINT msg , WPARAM wParam , LPARAM lParam);
	static LRESULT CALLBACK HandleMsgThunk (HWND hWnd , UINT msg , WPARAM wParam , LPARAM lParam); // 중간 다리 역할 해주는 함수(thunk) (인자로 받을 수 있는건 c 스타일의 전역 함수이기 때문에)
	LRESULT HandleMsg (HWND hWnd , UINT msg , WPARAM wParam , LPARAM lParam);

	int width;
	int height;
	HWND hWnd;
}; 