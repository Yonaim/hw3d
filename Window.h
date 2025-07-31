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
		WindowClass () noexcept; // �ܺο����� ���� �Ұ�
		~WindowClass ();
		WindowClass (const WindowClass&) = delete; // Do not use copy
		WindowClass& operator=(const WindowClass&) = delete; // Do not use copy
		static constexpr const wchar_t* wndClassName = L"Chili Direct3D Engine Window"; // ������ �ð� ���
		
		static WindowClass wndClass; // WindowClass Ŭ���� ���� WindowClass ��ü�� ���������� ���� (singleton)
		HINSTANCE hInst;
	};
public:
	Window (int width , int height , const wchar_t* name) noexcept;
	~Window ();
	Window (const Window&) = delete;
	Window& operator=(const Window&) = delete;
private:
	static LRESULT CALLBACK HandleMsgSetup (HWND hWnd , UINT msg , WPARAM wParam , LPARAM lParam);
	static LRESULT CALLBACK HandleMsgThunk (HWND hWnd , UINT msg , WPARAM wParam , LPARAM lParam); // �߰� �ٸ� ���� ���ִ� �Լ�(thunk) (���ڷ� ���� �� �ִ°� c ��Ÿ���� ���� �Լ��̱� ������)
	LRESULT HandleMsg (HWND hWnd , UINT msg , WPARAM wParam , LPARAM lParam);

	int width;
	int height;
	HWND hWnd;
}; 