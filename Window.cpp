#include "Window.h"

// ==================== Window::WindowClass =================

Window::WindowClass Window::WindowClass::wndClass; // ���� ��� ���� wndClass�� �޸𸮿� ����

Window::WindowClass::WindowClass () noexcept: hInst(GetModuleHandle(nullptr))
{
	// register window class
	WNDCLASSEXW wc = { 0 };
	wc.cbSize = sizeof (wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = HandleMsgSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetInstance();
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = GetName();
	wc.hIconSm = nullptr;
	RegisterClassExW (&wc);
}

Window::WindowClass::~WindowClass ()
{
	UnregisterClassW (wndClassName , GetInstance ());
}

const wchar_t* Window::WindowClass::GetName () noexcept
{
	return wndClassName;
}

HINSTANCE Window::WindowClass::GetInstance () noexcept
{
	return wndClass.hInst;
}

// ==================== Window =================
Window::Window (int width , int height , const wchar_t* name) noexcept
{
	// caculate window size based on desired client region size
	// Ŭ���̾�Ʈ ���� = ������ �׸��� ���� (Ÿ��Ʋ��, �׵θ� ����)
	RECT wr;
	wr.left = 100; // client region coordinates of screen space
	wr.top = 100;
	wr.right = width + wr.left;
	wr.bottom = height + wr.top;
	// ù��° ������ ���� Ÿ��Ʋ��, �׵θ� �����Ͽ� �ٲ���
	AdjustWindowRect (&wr , WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU , FALSE);

	// create window & get hWnd
	hWnd = CreateWindowW (WindowClass::GetName () , name , WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU , CW_USEDEFAULT , CW_USEDEFAULT , wr.right - wr.left , wr.bottom - wr.top , nullptr , nullptr , WindowClass::GetInstance() , this);
	ShowWindow (hWnd , SW_SHOWDEFAULT);
}

LRESULT CALLBACK Window::HandleMsgSetup (HWND hWnd , UINT msg , WPARAM wParam , LPARAM lParam) 
{
	if (msg == WM_NCCREATE) // non-client region create (�� ó��)
	{
		const CREATESTRUCTW* const pCreate = reinterpret_cast< CREATESTRUCTW* >( lParam );
		Window* const pWnd = static_cast< Window* >( pCreate->lpCreateParams );

		// ���� ������ ������ ��ü ������ ���
		SetWindowLongPtr (hWnd , GWLP_USERDATA , reinterpret_cast< LONG_PTR >( pWnd ));

		// ���� ȣ���, �޼��� ó�� (���ν���) �Լ��� �Ʒ��� thunk
		// lpfnWndProc ���� �����
		SetWindowLongPtr (hWnd , GWLP_WNDPROC , reinterpret_cast< LONG_PTR >( &Window::HandleMsgThunk ));
		return pWnd->HandleMsg (hWnd , msg , wParam , lParam);
	}
	else
		return DefWindowProc (hWnd , msg , wParam , lParam);
}

LRESULT CALLBACK Window::HandleMsgThunk (HWND hWnd , UINT msg , WPARAM wParam , LPARAM lParam) 
{
	// retrieve ptr to window instance
	Window* const pWnd = reinterpret_cast< Window* >( GetWindowLongPtr (hWnd , GWLP_USERDATA) );
	// forward message to window instance handler
	return pWnd->HandleMsg (hWnd , msg , wParam , lParam);
}

LRESULT CALLBACK Window::HandleMsg (HWND hWnd , UINT msg , WPARAM wParam , LPARAM lParam) 
{
	// ���� �����Ϳ� �־���� ��ü �����͸� �����
	//Window* const pWnd = reinterpret_cast< Window* >( GetWindowLongPtr (hWnd , GWLP_USERDATA) );
	//return pWnd->HandleMsg (hWnd , msg , wParam , lParam);

	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage (0);
		return ( 0 );
	}
	return DefWindowProc (hWnd , msg , wParam , lParam);
}

Window::~Window () { }